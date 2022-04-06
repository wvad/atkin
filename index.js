const { fork } = require('child_process');
const { deserialize } = require('v8');
const atkin = require('bindings')('addon');

module.exports = LIMIT => new Promise((resolve, reject) => {
  if (Number.isSafeInteger(LIMIT) && LIMIT < 430000) return resolve(atkin(LIMIT));
  const p = fork(__dirname + '/worker.js');
  const onExit = (code, signal) => {
    p.off('message', onMessage);
    p.off('error', onError);
    if (code === null) {
      reject(new Error(`The process was killed with signal '${signal}'`));
    }
  };
  const onError = err => {
    p.off('exit', onExit);
    p.off('message', onMessage);
    reject(err);
  };
  const onMessage = msg => {
    p.off('exit', onExit);
    p.off('error', onError);
    if (msg.ok) return resolve(msg.data);
    reject(deserialize(Buffer.from(msg.error.data)));
  }
  p.once('message', onMessage);
  p.once('error', onError);
  p.once('exit', onExit);
  p.send(LIMIT);
});

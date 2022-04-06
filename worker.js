const atkin = require('bindings')('addon');
const { serialize } = require('v8');

process.on('message', max => {
  try {
    const result = atkin(max);
    process.send({ ok: true, data: result });
  } catch (e) {
    process.send({ error: serialize(e) });
  }
});

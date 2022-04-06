# atkin

## Usage
```js
const atkin = require("atkin");

const MAX_VALUE = 10000000;

atkin(MAX_VALUE)
.then(array => {
  // array contains prime numbers that are less than MAX_VALUE.
  console.log("Prime numbers:", array);
})
.catch(exception => {
  console.error(exception);
});
```

console.log(0 == ''); // true
console.log(0 == '0'); // true
console.log(false == 'false'); // false; (''Boolean to string'')
console.log(false == '0'); // true (''Boolean to string'')
console.log(false == undefined); // false
console.log(false == null); // false (''Boolean to null'')
console.log(null == undefined); // true

console.log(0 === ''); // false
console.log(0 === '0'); // false
console.log(false === 'false'); // false
console.log(false === '0'); // false
console.log(false === undefined); // false
console.log(false === null); // false
console.log(null === undefined); // false

function f() {
  var x = 1;
  this.y = 2;
  z = 2;
}

f();

console.log(z);
console.log(y);
console.log(x);

function Factory() {
  this.x = 3;
  this.y = 4;
  this.r = 5;
}

f = new Factory;

console.log(f.x);
console.log(f.y);
console.log(f.r);

assertEq(f.x, 3);
assertEq(f.y, 4);
assertEq(f.r, 5);

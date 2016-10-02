a = "Hello";
Object.getPrototypeOf(a);

var p = {x: 1};
var o = Object.create(p);

Object.getPrototypeOf(o);

p.isPrototypeOf(o);

Object.prototype.isPrototypeOf(p);

var random = {
  get octet() { return Math.floor(Math.random()*256); },
  get uint16() { return Math.floor(Math.random()*65536); },
  get int16() { return Math.floor(Math.random()*65536) - 32768; },
};

random.octet;
random.uint16;
random.int16;

Object.getOwnPropertyDescriptor(random, "octet");

Object.getOwnPropertyDescriptor({}, "x");
Object.getOwnPropertyDescriptor({}, "toString");

var o = {};

Object.defineProperty(o, "x", { value:1, writable:true, enumerable:true, configurable:true });

assert(o.x == 1);

assert(Object.keys("o") == []);

Object.defineProperty(o, "x", { writable:false });

o.x = 2;

assert(o.x == 1);

Object.defineProperty(o, "x", { value: 2 });

assert(o.x == 2);

Object.defineProperty(o, "x", { get: function() { return 0; } });

assert(o.x == 0);

var p = Object.defineProperties({}, {
  x: { value:3, writeable:true, enumerable:true, configurable:true },
  y: { value:4, writeable:true, enumerable:true, configurable:true },
  r: { get: function() { return Math.sqrt(this.x*this.x + this.y*this.y) },
       enumerable:true, configurable:true }
});

assert(p.x == 3);
assert(p.y == 4);
assert(p.r == 5);

var q = Object.create({}, {
  x: { value:3, writeable:true, enumerable:true, configurable:true },
  y: { value:4, writeable:true, enumerable:true, configurable:true },
  r: { get: function() { return Math.sqrt(this.x*this.x + this.y*this.y) },
       enumerable:true, configurable:true }
});

assert(q.x == 3);
assert(q.y == 4);
assert(q.r == 5);

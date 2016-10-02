function f(y) { return this.x + y; }

var o = {x: 1};

var g = f.bind(o);

res = g(2);

assert(res == 3);

var sum = function(x,y) { return x + y; }
var succ = sum.bind(null, 1);
res = succ(2);

assert(res == 3);

function f(y, z) { return this.x + y + z; }
var g = f.bind({x:1}, 2);
res = g(3);

assert(res == 6);

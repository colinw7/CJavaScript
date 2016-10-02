a = [4,5,7,9,2];

b = a.slice(1,3);

assert(b, [5, 7]);

b = a.slice(-5,-2);

assert(b, [4, 5, 7]);

//---

var myFish = ['angel', 'clown', 'mandarin', 'surgeon'];

// removes 0 elements from index 2, and inserts 'drum'
var removed = myFish.splice(2, 0, 'drum');
assert(myFish == ['angel', 'clown', 'drum', 'mandarin', 'surgeon']);
assert(removed.length == 0);

// removes 1 element from index 3
removed = myFish.splice(3, 1);
assert(myFish == ['angel', 'clown', 'drum', 'surgeon']);
assert(removed == ['mandarin']);

// removes 1 element from index 2, and inserts 'trumpet'
removed = myFish.splice(2, 1, 'trumpet');
assert(myFish == ['angel', 'clown', 'trumpet', 'surgeon']);
assert(removed == ['drum']);

// removes 2 elements from index 0, and inserts 'parrot', 'anemone' and 'blue'
removed = myFish.splice(0, 2, 'parrot', 'anemone', 'blue');
assert(myFish == ['parrot', 'anemone', 'blue', 'trumpet', 'surgeon']);
assert(removed == ['angel', 'clown']);

// removes 2 elements from index 2
removed = myFish.splice(myFish.length -3, 2);
assert(myFish == ['parrot', 'anemone', 'surgeon']);
assert(removed == ['blue', 'trumpet']);

//---

var data = [1,2,3,4,5];

var sum = 0;

data.forEach(function(value) { sum += value; });
assert(sum == 15);

data.forEach(function(v, i, a) { a[i] = v + 1; });
assert(data == [2,3,4,5,6]);

function foreach(a,f,t) {
  try { a.forEach(f, t); }
  catch (e) {
    if (e == foreach.brk) return;
    else throw e;
  }
}

foreach.brk = new Error("StopIteration");

foreach(data,function(value) { fred[0]; },{});

//---

a = [1,2,3];
b = a.map(function(x) { return x*x; });
assert(b == [1,4,9]);

//---

a=[5,4,3,2,1];
smallValues = a.filter(function(x) { return x < 3; });
assert(smallValues == [2,1]);
everyOther = a.filter(function(x,i) { return i%2 == 0; });
assert(smallValues == [5,3,1]);

//---

a = [1,2,3,4,5];
res = a.every(function(x) { return x < 10; });
assert(res);
res = a.every(function(x) { return x % 2 === 0; });
assert(! res);

//---

a = [1,2,3,4,5];
res = a.some(function(x) { return x > 10; });
assert(! res);
res = a.some(function(x) { return x % 2 === 0; });
assert(res);

//---

a = [1,2,3,4,5];
var sum = a.reduce(function(x,y) { return x + y; }, 0);
assert(sum == 15);
var product = a.reduce(function(x,y) { return x * y; }, 1);
assert(product == 120);
var max = a.reduce(function(x,y) { return (x>y)?x:y; });
assert(max == 5);

a = [3,2,2]
var big = a.reduceRight(function(accumulator,value) { return Math.pow(value,accumulator); });
assert(big == 81);

function extend(o, p) {
  for (var prop in p) {
    o[prop] = p[prop];
  }
  return o;
}

function union(o,p) {
  return extend(extend({},o), p);
}

var objects = [{x:1}, {y:2}, {z:3}];
var merged = objects.reduce(union);
assert(merged, {x:'1', y:'2', z:'3' });

var objects = [{x:1,a:1}, {y:2,a:2}, {z:3,a:3}];
var leftunion = objects.reduce(union);
var rightunion = objects.reduceRight(union);

assert(leftunion, {a: '3', x: '1', y: '2', z: '3' });
assert(rightunion, {a: '1', x: '1', y: '2', z: '3' });

//---

a = [1,3,4,6,4,2];

i = a.indexOf(4);
assert(i == 2);

i = a.lastIndexOf(4);
assert(i == 4);

//---

b = Array.isArray([]);
assert(b);
b = Array.isArray({});
assert(!b);
b = [] instanceof Array;
assert(b);
b = ({}) instanceof Array;
assert(!b);

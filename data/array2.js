var a = {};

var i = 0;
while (i < 10) {
  a[i] = i*i;
  i++;
}
a.length = i;

var total = 0;
for (var j = 0; j < a.length; ++j) {
  total += a[j];
}

assert(total==285);

//----

var a = {"0":"a", "1":"b", "2":"c", length:3};
var b = Array.from(a);
assert(b == ['a','b','c']);

//----

var a = {"0":"a", "1":"b", "2":"c", length:3};
b = Array.prototype.join.call(a, "+");
assert(b == "a+b+c");

b = Array.prototype.slice.call(a, 0);
assert(b == ['a','b','c']);

b = Array.prototype.map.call(a, function(x) { return x.toUpperCase(); });
assert(b == ['A','B','C']);

var s = "JavaScript";
b = Array.prototype.join.call(s, " ");
assert(b == 'J a v a S c r i p t');

b = Array.prototype.filter.call(s, function(x) { return x.match(/[^aeiou]/); }).join("");
assert(b == 'JvScrpt');

a = [1,2,8,5,6];

m = Math.max.apply(Math, a);
assert(m == 8);

m = Math.max.call(Math, 1, 2, 8, 5, 6);
assert(m == 8);

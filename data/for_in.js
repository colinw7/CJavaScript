var simple_obj = {a: 1, b: 2, c: 3, d: 4};

for (var prop_of_simple_obj in simple_obj) {
  simple_obj[prop_of_simple_obj] += 4;
}

console.log(simple_obj.a, simple_obj.b, simple_obj.c, simple_obj.d);

a = [1, 2, 3];
b = {0: "zero", 1: "one", 2: "two", 3: "three"};

console.log(a);
console.log(b);

for (i in a) {
  console.log(i);
}

for (i in b) {
  console.log(i);
}

c = 1;

console.log(c in a);
console.log(c in b);

for (i in c in b) {
  console.log(i);
}

a1 = new Array();
a2 = new Array(4);
a3 = new Array(7, 2, 0, 4, -7);

console.log(a1);
console.log(a2);
console.log(a3);

console.log(a1.length);
console.log(a2.length);
console.log(a3.length);

console.log(a3.join());
console.log(a3.join(":"));

console.log(a3.reverse());
console.log(a3.sort());

a3[8] = "8";

console.log(a3);
console.log(a3.length);

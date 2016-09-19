delete a;

a = [1,2,3];
console.log(a);
delete a[1];
console.log(a);
delete a;
try {
console.log(a);
} catch (e) {
}

a = {a:"one",b:"two",c:"three",d:"four",e:"five"};
console.log(a);
delete a["a"];
console.log(a);
delete a.b;
console.log(a);
delete a;
try {
console.log(a);
} catch (e) {
}

a = "Hello";
console.log(a);
delete a[1]
console.log(a);
delete a;
try {
console.log(a);
} catch (e) {
}

a = [[1,2],[3,4],[5,6]];
console.log(a);
delete a[1];
console.log(a);
delete a[0][1];
console.log(a);
delete a;
try {
console.log(a);
} catch (e) {
}

a = {a:{a:"one",b:"two",c:"three"},b:{a:"one",b:"two",c:"three"},c:{a:"one",b:"two",c:"three"},d:{a:"one",b:"two",c:"three"},e:{a:"one",b:"two",c:"three"}};
console.log(a);
delete a["a"];
console.log(a);
delete a.b;
console.log(a);
delete a["c"]["a"];
console.log(a);
delete a.d.b;
console.log(a);
delete a;
try {
console.log(a);
} catch (e) {
}

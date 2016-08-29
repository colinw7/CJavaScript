Object.getOwnPropertyNames(RegExp);

a = RegExp("ab*");

console.log(a);

console.log(a.test("abc"));

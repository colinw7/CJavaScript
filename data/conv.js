num = 0;

function func() { }

// toString
console.log(1 + '2');
console.log(true + '3');
console.log(console + 1);
console.log(console + true);
console.log(null + '3');

// toNumber
console.log(Math.sin("1.45"));
console.log(num > "10");
console.log(num + true);
console.log(num - "3");

// toBool
console.log(0 ? "true" : "false");
console.log(1 ? "true" : "false");
console.log(-1 ? "true" : "false");
console.log("" ? "true" : "false");
console.log("a" ? "true" : "false");
console.log(null ? "true" : "false");
console.log(console ? "true" : "false");
console.log(func ? "true" : "false");

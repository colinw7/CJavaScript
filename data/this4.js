a = [1,2,3];

function f(x) { return x*x; }

console.log(a);
console.log(this.a[1]);
console.log(this.f(5));

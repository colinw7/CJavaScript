function Func() {
  return this;
}

Func.prototype.a = 1;

f = new Func();

console.log(f.a);

function one() {
  a = 1;

  function two() {
    return a;
  }

  return two;
}

f = one();

console.log(f);
console.log(f());

function setArray() {
  a=1;
  b=2;
  c=3;

  d1 = { a:a, b:b, c:c };

  a++;
  b++;
  c++;

  d2 = { a:a, b:b, c:c };

  d.push(d1);
  d.push(d2);
}

d = [];

setArray();

console.log(d);
console.log(d.length);
console.log(d[0]);
console.log(d[0].b);

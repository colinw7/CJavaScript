function setDict() {
  var d1 = {width:100, height:200};

  a=1;
  b=2;
  c=3;

  d = { a:a, b:b, c:c, width:d1.width, height:d1.height };
}

d = {};

setDict();

console.log(d);

console.log(d.a);
console.log(d.width);

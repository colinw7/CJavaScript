a=[];

a.push("one");
a.push("two");
a.push("three");

for (var i in a) {
  console.log(i + a[i]);
}

for (i in a) {
  console.log(i + a[i]);
}

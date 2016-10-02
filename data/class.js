function classOf(o) {
  if (o === null) return "Null";
  if (o === undefined) return "Undefined";
  return Object.prototype.toString.call(o).slice(8, -1);
}

console.log(classOf(null));
console.log(classOf(1));
console.log(classOf(""));
console.log(classOf(false));
console.log(classOf({}));
console.log(classOf([]));
console.log(classOf(/./));
console.log(classOf(new Date()));

//classOf(window);

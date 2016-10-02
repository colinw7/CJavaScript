function trace(o, m) {
  var original = o[m];
  o[m] = function() {
    console.log(new Date(), 'Entering:', m);
    var result = original.apply(this, arguments);
    console.log(new Date(), 'Exiting:', m);
    return result;
  };
}

a = new String("Hello");

trace(a, "toUpperCase");

console.log(a.toUpperCase());

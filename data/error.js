try {
  null[1] = 'abcd';
  
  assert (false);
} catch (e) {
  console.log("catch");

  assert(e instanceof TypeError);
  assert(e instanceof Error);
  assert(e instanceof Object);

  assert(!(e instanceof Function));
}

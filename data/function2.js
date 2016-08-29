function Test() {
  a = 1;
  console.log(a);
  try {
    console.log(b);
  } catch (e) {
    console.log("null");
  }
}

console.log("Test1");

Test();

a = 2;
b = 3;

console.log("Test2");

Test();

Test.a = 4;
Test.b = 4;

console.log("Test3");

Test();

console.log("Test4");

console.log(Test.a);
console.log(Test.b);

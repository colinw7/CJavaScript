f();

g();

function f() { console.log("f"); }

function g() {
  g1();

  function g1() { console.log("g1"); }

  g2();

  function g2() { console.log("g2"); }

  g1();
  g2();
}

f();

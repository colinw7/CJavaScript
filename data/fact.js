console.log("<h2>Table of factorials</h2>");

for (i = 1, fact = 1; i < 10; i++, fact *= i) {
  console.log(i + "! = " + fact);
  console.log("<br>");
}

x  = 100;
dx = 4;

ballRadius = 10;

width = 100;

if (x + dx > width-ballRadius || x + dx < ballRadius) {
  console.log("hit");
}

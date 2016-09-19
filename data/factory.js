var w = 100;
var h = 100;

function Factory(){
  this.x = Math.round( Math.random() * w);
  this.y = Math.round( Math.random() * h);
  this.r = Math.round( Math.random() * 1) + 1;
}

var particles = [];

for (var i = 0; i < 10; i++) {
  particles.push(new Factory);
}

console.log(particles);

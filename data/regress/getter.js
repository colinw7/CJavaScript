var p = {
  x: 1.0,
  y: 1.0,

  get r() { return Math.sqrt(this.x*this.x + this.y*this.y); },

  set r(new_r) {
    var old_r = Math.sqrt(this.x*this.x + this.y*this.y);
    var ratio = new_r/old_r;
    this.x *= ratio;
    this.y *= ratio;
  },

  get theta() { return Math.atan2(this.y, this.x); }
};

console.log(p.x);
console.log(p.y);
console.log(p.r);
console.log(p.theta);

p.x = 3;
p.y = 4;

console.log(p.x);
console.log(p.y);
console.log(p.r);
console.log(p.theta);

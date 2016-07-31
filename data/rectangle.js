function Rectangle_area() { return this.width * this.height; }
function Rectangle_perimeter() { return 2*this.width + 2*this.height; }
function Rectangle_set_size(w, h) { this.width = w; this.height = h; }
function Rectangle_enlarge() { this.width *= 2; this.height *= 2; }
function Rectangle_shrink() { this.width /= 2; this.height /= 2; }

function Rectangle(w, h)
{
  this.width  = w;
  this.height = h;

  this.area      = Rectangle_area;
  this.perimeter = Rectangle_perimeter;
  this.set_size  = Rectangle_set_size;
  this.enlarge   = Rectangle_enlarge;
  this.shrink    = Rectangle_shrink;
}

r = new Rectangle(2, 2);

console.log(r);

a = r.area();

console.log(a);

r.enlarge();

p = r.perimeter();

console.log(p);

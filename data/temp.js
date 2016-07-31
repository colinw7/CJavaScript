var eps = 1 / 65535; // 16.16 fixpoint epsilon
var passed = false;
    
function isequal( value, expected, epsilon ) {
  return(Math.abs(value - expected) < epsilon);
}

try {
  var g = document.getElementById("g");
  var c = document.getElementById("c");
  var tfm = g.transform.baseVal.getItem(0);
  var mtx = tfm.matrix;
  tfm.setTranslate(300,200);
  if (tfm.type == SVGTransform.SVG_TRANSFORM_TRANSLATE &&
      isequal(mtx.e, 300, eps) && isequal(mtx.f, 200, eps)) {
    tfm.setScale(4,4);
    if (tfm.type == SVGTransform.SVG_TRANSFORM_SCALE &&
        isequal(mtx.a, 4, eps) && isequal(mtx.d, 4, eps)) {
      tfm.setRotate(90,0,0);
      if (tfm.type == SVGTransform.SVG_TRANSFORM_ROTATE &&
          isequal(mtx.a, Math.cos(Math.PI/2), eps) && isequal(mtx.b, Math.sin(Math.PI/2), eps) &&
          isequal(mtx.c, -Math.sin(Math.PI/2), eps) && isequal(mtx.d, Math.cos(Math.PI/2), eps) &&
          isequal(mtx.e, 0, eps) && isequal(mtx.f, 0, eps)) {
        passed = true;
      }
    }
  }
}
catch(e) {}
    
if(passed) {
  c.setAttribute("fill", "lime");
} else {
  c.setAttribute("fill", "red");
}

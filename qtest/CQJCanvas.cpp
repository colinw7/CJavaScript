#include <CQJCanvas.h>
#include <CQJavaScript.h>
#include <CJavaScript.h>
#include <CQJImage.h>
#include <CStrParse.h>

namespace {
  QColor nameToColor(const std::string &name) {
    CStrParse parse(name);

    parse.skipSpace();

    if (parse.isString("rgb(")) {
      parse.skipChars(4);

      int r = 0, g = 0, b = 0;

      parse.skipSpace();
      parse.readInteger(&r);
      parse.skipSpace();

      if (parse.isChar(',')) {
        parse.skipChar();

        parse.skipSpace();
        parse.readInteger(&g);
        parse.skipSpace();

        if (parse.isChar(',')) {
          parse.skipChar();

          parse.skipSpace();
          parse.readInteger(&b);
          parse.skipSpace();

          if (parse.isChar(')'))
            parse.skipChar();
        }
      }

      QColor c(r, g, b);

      return c;
    }
    else {
      QColor c(name.c_str());

      return c;
    }
  }
}

//----

CQJCanvasType::
CQJCanvasType() :
 CJObjectType(CJToken::Type::Object, "Canvas")
{
}

CJValueP
CQJCanvasType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CQJCanvas *canvas = values[0]->cast<CQJCanvas>();
  assert(canvas);

  return CJValueP();
}

CQJCanvas::
CQJCanvas(CQJavaScript *qjs) :
 CJObject(qjs->jsCanvasType()), js_(qjs)
{
  CJavaScript *js = qjs->js();

  type_->addFunction(js, "getContext");

  updateSize();
}

void
CQJCanvas::
updateSize()
{
  CJavaScript *js = js_->js();

  setRealProperty(js, "width" , js_->canvas()->width ());
  setRealProperty(js, "height", js_->canvas()->height());
}

CJValueP
CQJCanvas::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if (name == "getContext") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      if (id == "2d")
        return js_->jsCanvasContext2D();
    }

    return CJValueP();
  }
  else
    return CJValueP();
}

//------

CQJCanvasContext2DType::
CQJCanvasContext2DType() :
 CJObjectType(CJToken::Type::Object, "CanvasContext2D")
{
}

CQJCanvasContext2D::
CQJCanvasContext2D(CQJavaScript *qjs) :
 CJObject(qjs->jsCanvasContext2DType()), js_(qjs)
{
  CJavaScript *js = qjs->js();

  type_->addFunction(js, "arc"         );
  type_->addFunction(js, "beginPath"   );
  type_->addFunction(js, "clearRect"   );
  type_->addFunction(js, "clip"        );
  type_->addFunction(js, "closePath"   );
  type_->addFunction(js, "fillRect"    );
  type_->addFunction(js, "measureText" );
  type_->addFunction(js, "fillText"    );
  type_->addFunction(js, "strokeText"  );
  type_->addFunction(js, "drawImage"   );
  type_->addFunction(js, "lineTo"      );
  type_->addFunction(js, "moveTo"      );
  type_->addFunction(js, "rect"        );
  type_->addFunction(js, "restore"     );
  type_->addFunction(js, "rotate"      );
  type_->addFunction(js, "save"        );
  type_->addFunction(js, "scale"       );
  type_->addFunction(js, "setTransform");
  type_->addFunction(js, "stroke"      );
  type_->addFunction(js, "fill"        );
  type_->addFunction(js, "strokeRect"  );
  type_->addFunction(js, "translate"   );

  type_->addFunction(js, "createLinearGradient");
  type_->addFunction(js, "createRadialGradient");
  type_->addFunction(js, "createPattern");

  setStringProperty(js, "fillStyle"  , "black");
  setStringProperty(js, "strokeStyle", "black");

  setRealProperty  (js, "lineWidth", 0);
  setStringProperty(js, "lineJoin" , "miter");
  setStringProperty(js, "lineCap"  , "square");

  setStringProperty(js, "font", "");

  setRealProperty  (js, "shadowOffsetX", 0);
  setRealProperty  (js, "shadowOffsetY", 0);
  setStringProperty(js, "shadowColor"  , "black");
  setRealProperty  (js, "shadowBlur"   , 0);

  setRealProperty(js, "globalAlpha", 1.0);

  setStringProperty(js, "globalCompositeOperation", "source-over");
}

CJValueP
CQJCanvasContext2D::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if      (name == "fillRect") {
    initFill();

    if (values.size() == 5) {
      double x = (values[1] ? values[1]->toReal() : 0);
      double y = (values[2] ? values[2]->toReal() : 0);
      double w = (values[3] ? values[3]->toReal() : 0);
      double h = (values[4] ? values[4]->toReal() : 0);

      js_->canvas()->fillRect(x, y, w, h);
    }
  }
  else if (name == "strokeRect") {
    initStroke();

    if (values.size() == 5) {
      double x = (values[1] ? values[1]->toReal() : 0);
      double y = (values[2] ? values[2]->toReal() : 0);
      double w = (values[3] ? values[3]->toReal() : 0);
      double h = (values[4] ? values[4]->toReal() : 0);

      js_->canvas()->moveTo(x    , y    );
      js_->canvas()->lineTo(x + w, y    );
      js_->canvas()->lineTo(x + w, y + h);
      js_->canvas()->lineTo(x    , y + h);

      js_->canvas()->closePath();

      js_->canvas()->stroke();
    }
  }
  else if (name == "clearRect") {
    if (values.size() == 5) {
      double x = (values[1] ? values[1]->toReal() : 0.0);
      double y = (values[2] ? values[2]->toReal() : 0.0);
      double w = (values[3] ? values[3]->toReal() : 0.0);
      double h = (values[4] ? values[4]->toReal() : 0.0);

      js_->canvas()->clearRect(x, y, w, h);
    }
  }
  else if (name == "measureText") {
    if (values.size() == 2) {
      std::string msg = values[1]->toString();

      return CJValueP(new CQJCanvasFontMetrics(js_, js_->canvas()->font(), msg));
    }
  }
  else if (name == "fillText") {
    if (values.size() == 4) {
      std::string text = (values[1] ? values[1]->toString() : "");
      double      x    = (values[2] ? values[2]->toReal() : 0.0);
      double      y    = (values[3] ? values[3]->toReal() : 0.0);

      js_->canvas()->fillText(x, y, text);
    }
  }
  else if (name == "strokeText") {
    if (values.size() == 4) {
      std::string text = (values[1] ? values[1]->toString() : "");
      double      x    = (values[2] ? values[2]->toReal() : 0.0);
      double      y    = (values[3] ? values[3]->toReal() : 0.0);

      js_->canvas()->strokeText(x, y, text);
    }
  }
  else if (name == "drawImage") {
    if (values.size() == 4) {
      CJValueP imageVal = values[1];
      double   x        = (values[2] ? values[2]->toReal() : 0.0);
      double   y        = (values[3] ? values[3]->toReal() : 0.0);

      if (imageVal && imageVal->type() == CJValue::Type::Object) {
        CJObjectP imageObj = std::static_pointer_cast<CJObject>(imageVal);

        if      (imageObj->type()->name() == "Image") {
          CQJImage *image = imageObj->cast<CQJImage>();

          js_->canvas()->drawImage(x, y, image->qimage());
        }
      }
    }
  }
  else if (name == "beginPath") {
    js_->canvas()->beginPath();
  }
  else if (name == "moveTo") {
    if (values.size() == 3) {
      double x = values[1]->toReal();
      double y = values[2]->toReal();

      js_->canvas()->moveTo(x, y);
    }
  }
  else if (name == "lineTo") {
    if (values.size() == 3) {
      double x = values[1]->toReal();
      double y = values[2]->toReal();

      js_->canvas()->lineTo(x, y);
    }
  }
  else if (name == "arc") {
    if (values.size() == 7) {
      double x       = values[1]->toReal();
      double y       = values[2]->toReal();
      double r       = values[3]->toReal();
      double a1      = values[4]->toReal();
      double a2      = values[5]->toReal();
      double counter = values[6]->toBoolean();

      js_->canvas()->arc(x, y, r, a1, a2, counter);
    }
  }
  else if (name == "closePath") {
    js_->canvas()->closePath();
  }
  else if (name == "stroke") {
    initStroke();

    js_->canvas()->stroke();
  }
  else if (name == "fill") {
    initFill();

    js_->canvas()->fill();
  }
  else if (name == "save") {
    js_->canvas()->save();
  }
  else if (name == "restore") {
    js_->canvas()->restore();
  }
  else if (name == "rect") {
    if (values.size() == 5) {
      double x = values[1]->toReal();
      double y = values[2]->toReal();
      double w = values[3]->toReal();
      double h = values[4]->toReal();

      js_->canvas()->rect(x, y, w, h);
    }
  }
  else if (name == "clip") {
    js_->canvas()->clip();
  }
  else if (name == "setTransform") {
    if (values.size() == 7) {
      double a = values[1]->toReal();
      double b = values[2]->toReal();
      double c = values[3]->toReal();
      double d = values[4]->toReal();
      double e = values[5]->toReal();
      double f = values[6]->toReal();

      js_->canvas()->setTransform(a, b, c, d, e, f);
    }
  }
  else if (name == "translate") {
    if (values.size() == 3) {
      double dx = values[1]->toReal();
      double dy = values[2]->toReal();

      js_->canvas()->translate(dx, dy);
    }
  }
  else if (name == "scale") {
    if (values.size() == 3) {
      double xs = values[1]->toReal();
      double ys = values[2]->toReal();

      js_->canvas()->scale(xs, ys);
    }
  }
  else if (name == "rotate") {
    if (values.size() == 2) {
      double a = values[1]->toReal();

      js_->canvas()->rotate(a);
    }
  }
  else if (name == "createLinearGradient") {
    if (values.size() == 5) {
      double x1 = values[1]->toReal();
      double y1 = values[2]->toReal();
      double x2 = values[3]->toReal();
      double y2 = values[4]->toReal();

      return CJValueP(new CQJCanvasLinearGradient(js_, x1, y1, x2, y2));
    }
  }
  else if (name == "createRadialGradient") {
    if (values.size() == 7) {
      double x1 = values[1]->toReal();
      double y1 = values[2]->toReal();
      double r1 = values[3]->toReal();
      double x2 = values[4]->toReal();
      double y2 = values[5]->toReal();
      double r2 = values[6]->toReal();

      return CJValueP(new CQJCanvasRadialGradient(js_, x1, y1, r1, x2, y2, r2));
    }
  }
  else if (name == "createPattern") {
    if (values.size() == 3) {
      CJValueP    patternVal = values[1];
      std::string repeatStr  = values[2]->toString();

      CQJCanvasPattern::Repeat repeat = CQJCanvasPattern::Repeat::None;

      if      (repeatStr == "no-repeat")
        repeat = CQJCanvasPattern::Repeat::None;
      else if (repeatStr == "repeat-x")
        repeat = CQJCanvasPattern::Repeat::X;
      else if (repeatStr == "repeat-y")
        repeat = CQJCanvasPattern::Repeat::Y;

      if (patternVal->type() == CJValue::Type::Object) {
        CJObjectP patternObj = std::static_pointer_cast<CJObject>(patternVal);

        if      (patternObj->type()->name() == "Image") {
          CQJImage *image = patternObj->cast<CQJImage>();

          return CJValueP(new CQJCanvasPattern(js_, image->qimage(), repeat));
        }
      }
    }
  }

  js_->canvas()->update();

  return CJValueP();
}

void
CQJCanvasContext2D::
initFill()
{
  CJValueP fillStyle = getProperty("fillStyle");

  if (fillStyle->type() == CJValue::Type::Object) {
    CJObjectP fillStyleObj = std::static_pointer_cast<CJObject>(fillStyle);

    if      (fillStyleObj->type()->name() == "CanvasLinearGradient") {
      CQJCanvasLinearGradient *lg = fillStyle->cast<CQJCanvasLinearGradient>();

      js_->canvas()->setFillGradient(lg->lg());
    }
    else if (fillStyleObj->type()->name() == "CanvasRadialGradient") {
      CQJCanvasRadialGradient *rg = fillStyle->cast<CQJCanvasRadialGradient>();

      js_->canvas()->setFillGradient(rg->rg());
    }
    else if (fillStyleObj->type()->name() == "CanvasPattern") {
      CQJCanvasPattern *pattern = fillStyle->cast<CQJCanvasPattern>();

      js_->canvas()->setFillPattern(pattern->qimage());
    }
  }
  else {
    std::string colorName = getStringProperty("fillStyle", "black");

    QColor c = nameToColor(colorName);

    js_->canvas()->setFillColor(c);
  }

  double alpha = getRealProperty("globalAlpha", 1.0);

  js_->canvas()->setFillAlpha(alpha);
}

void
CQJCanvasContext2D::
initStroke()
{
  CJValueP strokeStyle = getProperty("strokeStyle");

  if (strokeStyle->type() == CJValue::Type::Object) {
    CJObjectP strokeStyleObj = std::static_pointer_cast<CJObject>(strokeStyle);

    if      (strokeStyleObj->type()->name() == "CanvasLinearGradient") {
      CQJCanvasLinearGradient *lg = strokeStyle->cast<CQJCanvasLinearGradient>();

      js_->canvas()->setStrokeGradient(lg->lg());
    }
    else if (strokeStyleObj->type()->name() == "CanvasRadialGradient") {
      CQJCanvasRadialGradient *rg = strokeStyle->cast<CQJCanvasRadialGradient>();

      js_->canvas()->setStrokeGradient(rg->rg());
    }
  }
  else {
    std::string colorName = getStringProperty("strokeStyle", "black");

    QColor c = nameToColor(colorName);

    js_->canvas()->setStrokeColor(c);
  }

  double alpha = getRealProperty  ("globalAlpha", 1.0);
  double width = getRealProperty  ("lineWidth"  , 0);

  js_->canvas()->setStrokeAlpha(alpha);
  js_->canvas()->setStrokeWidth(width);

  std::string capName  = getStringProperty("lineCap" , "square");
  std::string joinName = getStringProperty("lineJoin", "miter" );

  if      (capName == "square")
    js_->canvas()->setStrokeCap(Qt::SquareCap);
  else if (capName == "round")
    js_->canvas()->setStrokeCap(Qt::RoundCap);
  else if (capName == "butt")
    js_->canvas()->setStrokeCap(Qt::FlatCap);

  if      (joinName == "bevel")
    js_->canvas()->setStrokeJoin(Qt::BevelJoin);
  else if (joinName == "round")
    js_->canvas()->setStrokeJoin(Qt::RoundJoin);
  else if (joinName == "miter")
    js_->canvas()->setStrokeJoin(Qt::MiterJoin);
}

//------

CQJCanvasLinearGradientType::
CQJCanvasLinearGradientType() :
 CJObjectType(CJToken::Type::Object, "CanvasLinearGradient")
{
}

CQJCanvasLinearGradient::
CQJCanvasLinearGradient(CQJavaScript *qjs, double x1, double y1, double x2, double y2) :
 CJObject(qjs->jsCanvasLinearGradientType()), js_(qjs)
{
  CJavaScript *js = qjs->js();

  type_->addFunction(js, "addColorStop");

  lg_ = QLinearGradient(x1, y1, x2, y2);
}

CJValueP
CQJCanvasLinearGradient::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if      (name == "addColorStop") {
    if (values.size() == 3) {
      double      r         = values[1]->toReal();
      std::string colorName = values[2]->toString();

      QColor c = nameToColor(colorName);

      lg_.setColorAt(r, c);
    }
  }

  return CJValueP();
}

//------

CQJCanvasRadialGradientType::
CQJCanvasRadialGradientType() :
 CJObjectType(CJToken::Type::Object, "CanvasRadialGradient")
{
}

CQJCanvasRadialGradient::
CQJCanvasRadialGradient(CQJavaScript *qjs, double x1, double y1, double r1,
                        double x2, double y2, double r2) :
 CJObject(qjs->jsCanvasRadialGradientType()), js_(qjs)
{
  CJavaScript *js = qjs->js();

  type_->addFunction(js, "addColorStop");

  rg_ = QRadialGradient(x2, y2, r2, x1, y1, r1);
}

CJValueP
CQJCanvasRadialGradient::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if      (name == "addColorStop") {
    if (values.size() == 3) {
      double      r         = values[1]->toReal();
      std::string colorName = values[2]->toString();

      QColor c = nameToColor(colorName);

      rg_.setColorAt(r, c);
    }
  }

  return CJValueP();
}

//------

CQJCanvasPatternType::
CQJCanvasPatternType() :
 CJObjectType(CJToken::Type::Object, "CanvasPattern")
{
}

CQJCanvasPattern::
CQJCanvasPattern(CQJavaScript *qjs, const QImage &qimage, const Repeat &repeat) :
 CJObject(qjs->jsCanvasPatternType()), js_(qjs), qimage_(qimage), repeat_(repeat)
{
}

CJValueP
CQJCanvasPattern::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CQJCanvasFontMetricsType::
CQJCanvasFontMetricsType() :
 CJObjectType(CJToken::Type::Object, "CanvasFontMetrics")
{
}

CQJCanvasFontMetrics::
CQJCanvasFontMetrics(CQJavaScript *qjs, const QFont &font, const std::string &text) :
 CJObject(qjs->jsCanvasFontMetricsType()), js_(qjs), text_(text), font_(font), fm_(font)
{
  CJavaScript *js = js_->js();

  setRealProperty(js, "width", fm_.width(text.c_str()));
}

CJValueP
CQJCanvasFontMetrics::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

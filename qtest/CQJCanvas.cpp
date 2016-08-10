#include <CQJCanvas.h>
#include <CQJSCanvas.h>
#include <CQJavaScript.h>
#include <CJavaScript.h>
#include <CQJImage.h>
#include <CStrParse.h>
#include <iostream>

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
 CQJObject(qjs, qjs->jsCanvasType())
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
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
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
    return CQJObject::execNameFn(js, name, values);
}

//------

CJObjectTypeP CQJCanvasContext2DType::type_;

CJObjectTypeP
CQJCanvasContext2DType::
instance(CQJavaScript *qjs)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CQJCanvasContext2DType());

    CJavaScript *js = qjs->js();

    js->addObjectType("CanvasContext2D", type_);
  }

  return type_;
}

CQJCanvasContext2DType::
CQJCanvasContext2DType() :
 CJObjectType(CJToken::Type::Object, "CanvasContext2D")
{
}

CQJCanvasContext2D::
CQJCanvasContext2D(CQJavaScript *qjs) :
 CJObject(CQJCanvasContext2DType::instance(qjs)), js_(qjs)
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

  type_->addFunction(js, "createImageData");
  type_->addFunction(js, "getImageData");
  type_->addFunction(js, "putImageData");

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

      js_->canvas()->update();

      return CJValueP();
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

      js_->canvas()->update();

      return CJValueP();
    }
  }
  else if (name == "clearRect") {
    if (values.size() == 5) {
      double x = (values[1] ? values[1]->toReal() : 0.0);
      double y = (values[2] ? values[2]->toReal() : 0.0);
      double w = (values[3] ? values[3]->toReal() : 0.0);
      double h = (values[4] ? values[4]->toReal() : 0.0);

      js_->canvas()->clearRect(x, y, w, h);

      js_->canvas()->update();

      return CJValueP();
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

      js_->canvas()->update();

      return CJValueP();
    }
  }
  else if (name == "strokeText") {
    if (values.size() == 4) {
      std::string text = (values[1] ? values[1]->toString() : "");
      double      x    = (values[2] ? values[2]->toReal() : 0.0);
      double      y    = (values[3] ? values[3]->toReal() : 0.0);

      js_->canvas()->strokeText(x, y, text);

      js_->canvas()->update();

      return CJValueP();
    }
  }
  else if (name == "drawImage") {
    // void ctx.drawImage(image, dx, dy);
    // void ctx.drawImage(image, dx, dy, dWidth, dHeight);
    // void ctx.drawImage(image, sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight);
    if (values.size() >= 4) {
      CJValueP imageVal = values[1];

      double sx = 0.0, sy = 0.0, sWidth = 0.0, sHeight = 0.0;
      double dx = 0.0, dy = 0.0, dWidth = 0.0, dHeight = 0.0;

      if      (values.size() == 4 || values.size() == 6) {
        dx = (values[2] ? values[2]->toReal() : 0.0);
        dy = (values[3] ? values[3]->toReal() : 0.0);

        if (values.size() == 6) {
          dWidth  = (values[4] ? values[4]->toReal() : 0.0);
          dHeight = (values[5] ? values[5]->toReal() : 0.0);
        }
      }
      else if (values.size() == 10) {
        sx      = (values[2] ? values[2]->toReal() : 0.0);
        sy      = (values[3] ? values[3]->toReal() : 0.0);
        sWidth  = (values[4] ? values[4]->toReal() : 0.0);
        sHeight = (values[5] ? values[5]->toReal() : 0.0);
        dx      = (values[6] ? values[6]->toReal() : 0.0);
        dy      = (values[7] ? values[7]->toReal() : 0.0);
        dWidth  = (values[8] ? values[8]->toReal() : 0.0);
        dHeight = (values[9] ? values[9]->toReal() : 0.0);
      }

      CQJImage *image = 0;

      if (imageVal && imageVal->type() == CJValue::Type::Object) {
        CJObjectP imageObj = std::static_pointer_cast<CJObject>(imageVal);

        if (imageObj->type()->name() == "Image") {
          image = imageObj->cast<CQJImage>();
        }
      }

      if (image) {
        if      (values.size() == 4)
          js_->canvas()->drawImage(image->qimage(), dx, dy);
        else if (values.size() == 6)
          js_->canvas()->drawImage(image->qimage(), dx, dy, dWidth, dHeight);
        else if (values.size() == 10)
          js_->canvas()->drawImage(image->qimage(), sx, sy, sWidth, sHeight,
                                   dx, dy, dWidth, dHeight);
        else
          std::cerr << "Invalid number of args for '" << name << "'" << std::endl;
      }
      else
        std::cerr << "Invalid image for '" << name << "'" << std::endl;

      js_->canvas()->update();

      return CJValueP();
    }
  }
  else if (name == "beginPath") {
    js_->canvas()->beginPath();

    return CJValueP();
  }
  else if (name == "moveTo") {
    if (values.size() == 3) {
      double x = values[1]->toReal();
      double y = values[2]->toReal();

      js_->canvas()->moveTo(x, y);

      return CJValueP();
    }
  }
  else if (name == "lineTo") {
    if (values.size() == 3) {
      double x = values[1]->toReal();
      double y = values[2]->toReal();

      js_->canvas()->lineTo(x, y);

      return CJValueP();
    }
  }
  else if (name == "arc") {
    if (values.size() == 7) {
      double x       = (values[1] ? values[1]->toReal() : 0);
      double y       = (values[2] ? values[2]->toReal() : 0);
      double r       = (values[3] ? values[3]->toReal() : 0);
      double a1      = (values[4] ? values[4]->toReal() : 0);
      double a2      = (values[5] ? values[5]->toReal() : 0);
      bool   counter = (values[6] ? values[6]->toBoolean() : false);

      js_->canvas()->arc(x, y, r, a1, a2, counter);

      return CJValueP();
    }
  }
  else if (name == "closePath") {
    js_->canvas()->closePath();

    return CJValueP();
  }
  else if (name == "stroke") {
    initStroke();

    js_->canvas()->stroke();

    js_->canvas()->update();

    return CJValueP();
  }
  else if (name == "fill") {
    initFill();

    js_->canvas()->fill();

    js_->canvas()->update();

    return CJValueP();
  }
  else if (name == "save") {
    js_->canvas()->save();

    return CJValueP();
  }
  else if (name == "restore") {
    js_->canvas()->restore();

    return CJValueP();
  }
  else if (name == "rect") {
    if (values.size() == 5) {
      double x = values[1]->toReal();
      double y = values[2]->toReal();
      double w = values[3]->toReal();
      double h = values[4]->toReal();

      js_->canvas()->rect(x, y, w, h);

      js_->canvas()->update();

      return CJValueP();
    }
  }
  else if (name == "clip") {
    js_->canvas()->clip();

    js_->canvas()->update();

    return CJValueP();
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

      return CJValueP();
    }
  }
  else if (name == "translate") {
    if (values.size() == 3) {
      double dx = values[1]->toReal();
      double dy = values[2]->toReal();

      js_->canvas()->translate(dx, dy);

      return CJValueP();
    }
  }
  else if (name == "scale") {
    if (values.size() == 3) {
      double xs = values[1]->toReal();
      double ys = values[2]->toReal();

      js_->canvas()->scale(xs, ys);

      return CJValueP();
    }
  }
  else if (name == "rotate") {
    if (values.size() == 2) {
      double a = values[1]->toReal();

      js_->canvas()->rotate(a);

      return CJValueP();
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
  else if (name == "createImageData") {
    if (values.size() == 3) {
      double w = values[1]->toReal();
      double h = values[2]->toReal();

      return CJValueP(new CQJCanvasImageData(js_, int(w), int(h)));
    }
  }
  else if (name == "getImageData") {
    if (values.size() == 5) {
      double x = values[1]->toReal();
      double y = values[2]->toReal();
      double w = values[3]->toReal();
      double h = values[4]->toReal();

      QImage image = js_->canvas()->getImageData(x, y, w, h);

      return CJValueP(new CQJCanvasImageData(js_, image));
    }
  }
  else if (name == "putImageData") {
    if (values.size() == 4) {
      CJValueP imageVal = values[1];
      double   x        = values[2]->toReal();
      double   y        = values[3]->toReal();

      if (imageVal->type() == CJValue::Type::Object) {
        CJObjectP imageObj = std::static_pointer_cast<CJObject>(imageVal);

        if      (imageObj->type()->name() == "Image") {
          CQJImage *image = imageObj->cast<CQJImage>();

          js_->canvas()->putImageData(image->qimage(), x, y);
        }
      }

      return CJValueP();
    }
  }

  std::cerr << "CQJCanvasContext2D function '" << name << "' failed" << std::endl;

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

CJObjectTypeP CQJCanvasLinearGradientType::type_;

CJObjectTypeP
CQJCanvasLinearGradientType::
instance()
{
  if (! type_)
    type_ = CJObjectTypeP(new CQJCanvasLinearGradientType());

  return type_;
}

CQJCanvasLinearGradientType::
CQJCanvasLinearGradientType() :
 CJObjectType(CJToken::Type::Object, "CanvasLinearGradient")
{
}

CQJCanvasLinearGradient::
CQJCanvasLinearGradient(CQJavaScript *qjs, double x1, double y1, double x2, double y2) :
 CJObject(CQJCanvasLinearGradientType::instance()), js_(qjs)
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

CJObjectTypeP CQJCanvasRadialGradientType::type_;

CJObjectTypeP
CQJCanvasRadialGradientType::
instance()
{
  if (! type_)
    type_ = CJObjectTypeP(new CQJCanvasRadialGradientType());

  return type_;
}

CQJCanvasRadialGradientType::
CQJCanvasRadialGradientType() :
 CJObjectType(CJToken::Type::Object, "CanvasRadialGradient")
{
}

CQJCanvasRadialGradient::
CQJCanvasRadialGradient(CQJavaScript *qjs, double x1, double y1, double r1,
                        double x2, double y2, double r2) :
 CJObject(CQJCanvasRadialGradientType::instance()), js_(qjs)
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

CJObjectTypeP CQJCanvasPatternType::type_;

CJObjectTypeP
CQJCanvasPatternType::
instance()
{
  if (! type_)
    type_ = CJObjectTypeP(new CQJCanvasPatternType());

  return type_;
}

CQJCanvasPatternType::
CQJCanvasPatternType() :
 CJObjectType(CJToken::Type::Object, "CanvasPattern")
{
}

CQJCanvasPattern::
CQJCanvasPattern(CQJavaScript *qjs, const QImage &qimage, const Repeat &repeat) :
 CJObject(CQJCanvasPatternType::instance()), js_(qjs), qimage_(qimage), repeat_(repeat)
{
}

CJValueP
CQJCanvasPattern::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJObjectTypeP CQJCanvasImageDataType::type_;

CJObjectTypeP
CQJCanvasImageDataType::
instance()
{
  if (! type_)
    type_ = CJObjectTypeP(new CQJCanvasImageDataType());

  return type_;
}

CQJCanvasImageDataType::
CQJCanvasImageDataType() :
 CJObjectType(CJToken::Type::Object, "CanvasImageData")
{
}

CQJCanvasImageData::
CQJCanvasImageData(CQJavaScript *qjs, int w, int h) :
 CQJObject(qjs, CQJCanvasImageDataType::instance())
{
  qimage_ = QImage(w, h, QImage::Format_ARGB32);

  CJavaScript *js = js_->js();

  setRealProperty(js, "width" , w);
  setRealProperty(js, "height", h);
}

CQJCanvasImageData::
CQJCanvasImageData(CQJavaScript *qjs, const QImage &image) :
 CQJObject(qjs, CQJCanvasImageDataType::instance()), qimage_(image)
{
  CJavaScript *js = js_->js();

  setRealProperty(js, "width" , image.width ());
  setRealProperty(js, "height", image.height());
}

CJValueP
CQJCanvasImageData::
getProperty(const std::string &key) const
{
  if (key == "data") {
    CQJCanvasImageData *th = const_cast<CQJCanvasImageData *>(this);

    CQJCanvasImageDataP data =
      std::static_pointer_cast<CQJCanvasImageData>(th->shared_from_this());

    return CJValueP(new CQJCanvasImageDataArray(js(), data));
  }
  else
    return CJObject::getProperty(key);
}

CJValueP
CQJCanvasImageData::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJObjectTypeP CQJCanvasImageDataArrayType::type_;

CJObjectTypeP
CQJCanvasImageDataArrayType::
instance()
{
  if (! type_)
    type_ = CJObjectTypeP(new CQJCanvasImageDataArrayType());

  return type_;
}

CQJCanvasImageDataArrayType::
CQJCanvasImageDataArrayType() :
 CJObjectType(CJToken::Type::Object, "CanvasImageDataArray")
{
}

CQJCanvasImageDataArray::
CQJCanvasImageDataArray(CQJavaScript *qjs, CQJCanvasImageDataP data) :
 CQJObject(qjs, CQJCanvasImageDataArrayType::instance()), data_(data)
{
  int w = data_->qimage().width ();
  int h = data_->qimage().height();

  CJavaScript *js = js_->js();

  setRealProperty(js, "length", 4*w*h);
}

CJValueP
CQJCanvasImageDataArray::
indexValue(int i) const
{
  int i1 = i / 4;
  int i0 = i % 4;

  int w = data_->qimage().width ();
  int h = data_->qimage().height();

  CJavaScript *js = js_->js();

  if (i1 < 0 || i1 >= w*h)
    //return CJValueP();
    return js->createNumberValue(0L);

  int y = i1 / w;
  int x = i1 % w;

  QRgb rgb = data_->getPixel(x, y);

  long c = 0;

  if      (i0 == 0) c = qRed  (rgb);
  else if (i0 == 1) c = qGreen(rgb);
  else if (i0 == 2) c = qBlue (rgb);
  else if (i0 == 3) c = qAlpha(rgb);

  return js->createNumberValue(c);
}

void
CQJCanvasImageDataArray::
setIndexValue(int i, CJValueP value)
{
  int i1 = i / 4;
  int i0 = i % 4;

  int w = data_->qimage().width ();
  int h = data_->qimage().height();

  if (i1 < 0 || i1 >= w*h)
    return;

  int y = i1 / w;
  int x = i1 % w;

  long c = value->toInteger();

  QRgb rgb = data_->getPixel(x, y);

  QRgb rgb1;

  if      (i0 == 0) rgb1 = qRgba(c        , qGreen(rgb), qBlue(rgb), qAlpha(rgb));
  else if (i0 == 1) rgb1 = qRgba(qRed(rgb), c          , qBlue(rgb), qAlpha(rgb));
  else if (i0 == 2) rgb1 = qRgba(qRed(rgb), qGreen(rgb), c         , qAlpha(rgb));
  else if (i0 == 3) rgb1 = qRgba(qRed(rgb), qGreen(rgb), qBlue(rgb), c          );

  data_->setPixel(x, y, rgb1);

}

CJValueP
CQJCanvasImageDataArray::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJObjectTypeP CQJCanvasFontMetricsType::type_;

CJObjectTypeP
CQJCanvasFontMetricsType::
instance()
{
  if (! type_)
    type_ = CJObjectTypeP(new CQJCanvasFontMetricsType());

  return type_;
}

CQJCanvasFontMetricsType::
CQJCanvasFontMetricsType() :
 CJObjectType(CJToken::Type::Object, "CanvasFontMetrics")
{
}

CQJCanvasFontMetrics::
CQJCanvasFontMetrics(CQJavaScript *qjs, const QFont &font, const std::string &text) :
 CQJObject(qjs, CQJCanvasFontMetricsType::instance()), text_(text), font_(font), fm_(font)
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

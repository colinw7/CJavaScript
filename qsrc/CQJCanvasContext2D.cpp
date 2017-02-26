#include <CQJCanvasContext2D.h>
#include <CQJCanvasFontMetrics.h>
#include <CQJCanvasLinearGradient.h>
#include <CQJCanvasRadialGradient.h>
#include <CQJCanvasPattern.h>
#include <CQJCanvasImageData.h>
#include <CQJCanvas.h>
#include <CQJCanvasWidget.h>
#include <CQJImage.h>
#include <CQJImageObj.h>
#include <CQJUtil.h>
#include <CJavaScript.h>

CJObjTypeP CQJCanvasContext2DType::type_;

CJObjTypeP
CQJCanvasContext2DType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJCanvasContext2DType(js));

    js->addObjectType("CanvasContext2D", type_);
  }

  return type_;
}

CQJCanvasContext2DType::
CQJCanvasContext2DType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "CanvasContext2D")
{
}

CQJCanvasContext2D::
CQJCanvasContext2D(CJavaScript *js, CQJCanvas *canvas) :
 CJObj(js, CQJCanvasContext2DType::instance(js)), canvas_(canvas)
{
  objType_->addObjFunction(js, "arc"         , objType_);
  objType_->addObjFunction(js, "beginPath"   , objType_);
  objType_->addObjFunction(js, "clearRect"   , objType_);
  objType_->addObjFunction(js, "clip"        , objType_);
  objType_->addObjFunction(js, "closePath"   , objType_);
  objType_->addObjFunction(js, "fillRect"    , objType_);
  objType_->addObjFunction(js, "measureText" , objType_);
  objType_->addObjFunction(js, "fillText"    , objType_);
  objType_->addObjFunction(js, "strokeText"  , objType_);
  objType_->addObjFunction(js, "drawImage"   , objType_);
  objType_->addObjFunction(js, "lineTo"      , objType_);
  objType_->addObjFunction(js, "moveTo"      , objType_);
  objType_->addObjFunction(js, "rect"        , objType_);
  objType_->addObjFunction(js, "restore"     , objType_);
  objType_->addObjFunction(js, "rotate"      , objType_);
  objType_->addObjFunction(js, "save"        , objType_);
  objType_->addObjFunction(js, "scale"       , objType_);
  objType_->addObjFunction(js, "setTransform", objType_);
  objType_->addObjFunction(js, "stroke"      , objType_);
  objType_->addObjFunction(js, "fill"        , objType_);
  objType_->addObjFunction(js, "strokeRect"  , objType_);
  objType_->addObjFunction(js, "translate"   , objType_);

  objType_->addObjFunction(js, "createLinearGradient", objType_);
  objType_->addObjFunction(js, "createRadialGradient", objType_);
  objType_->addObjFunction(js, "createPattern"       , objType_);

  objType_->addObjFunction(js, "createImageData", objType_);
  objType_->addObjFunction(js, "getImageData"   , objType_);
  objType_->addObjFunction(js, "putImageData"   , objType_);

  setStringProperty(js, "fillStyle"  , "black");
  setStringProperty(js, "strokeStyle", "black");

  setRealProperty  (js, "lineWidth", 0);
  setStringProperty(js, "lineJoin" , "miter");
  setStringProperty(js, "lineCap"  , "square");

  setStringProperty(js, "font", "12px sans-serif");
  setStringProperty(js, "textBaseline", "alphabetic");
  setStringProperty(js, "textAlign", "left");

  setRealProperty  (js, "shadowOffsetX", 0);
  setRealProperty  (js, "shadowOffsetY", 0);
  setStringProperty(js, "shadowColor"  , "black");
  setRealProperty  (js, "shadowBlur"   , 0);

  setRealProperty(js, "globalAlpha", 1.0);

  setStringProperty(js, "globalCompositeOperation", "source-over");
}

CQJCanvasWidget *
CQJCanvasContext2D::
canvasWidget() const
{
  return canvas_->canvasWidget();
}

CJValueP
CQJCanvasContext2D::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  CQJCanvasWidget *canvas = canvasWidget();

  if      (name == "fillRect") {
    initFill();

    if (values.size() == 5) {
      double x = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double y = (values[2] ? values[2]->toReal().getValue(0) : 0);
      double w = (values[3] ? values[3]->toReal().getValue(0) : 0);
      double h = (values[4] ? values[4]->toReal().getValue(0) : 0);

      canvas->fillRect(x, y, w, h);

      canvas->update();

      return CJValueP();
    }
  }
  else if (name == "strokeRect") {
    initStroke();

    if (values.size() == 5) {
      double x = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double y = (values[2] ? values[2]->toReal().getValue(0) : 0);
      double w = (values[3] ? values[3]->toReal().getValue(0) : 0);
      double h = (values[4] ? values[4]->toReal().getValue(0) : 0);

      canvas->beginPath();

      canvas->moveTo(x    , y    );
      canvas->lineTo(x + w, y    );
      canvas->lineTo(x + w, y + h);
      canvas->lineTo(x    , y + h);

      canvas->closePath();

      canvas->stroke();

      canvas->update();

      return CJValueP();
    }
  }
  else if (name == "clearRect") {
    if (values.size() == 5) {
      double x = (values[1] ? values[1]->toReal().getValue(0) : 0.0);
      double y = (values[2] ? values[2]->toReal().getValue(0) : 0.0);
      double w = (values[3] ? values[3]->toReal().getValue(0) : 0.0);
      double h = (values[4] ? values[4]->toReal().getValue(0) : 0.0);

      canvas->clearRect(x, y, w, h);

      canvas->update();

      return CJValueP();
    }
  }
  else if (name == "measureText") {
    if (values.size() == 2) {
      std::string msg = (values[1] ? values[1]->toString() : "");

      return CJValueP(new CQJCanvasFontMetrics(js, canvas->font(), msg));
    }
  }
  else if (name == "fillText") {
    initFont();

    initFill();

    if (values.size() == 4) {
      std::string text = (values[1] ? values[1]->toString() : "");
      double      x    = (values[2] ? values[2]->toReal().getValue(0) : 0.0);
      double      y    = (values[3] ? values[3]->toReal().getValue(0) : 0.0);

      double shadowBlur = getRealProperty(js, "shadowBlur", 0);

      if (shadowBlur > 0) {
        double shadowX = getRealProperty(js, "shadowOffsetX", 0);
        double shadowY = getRealProperty(js, "shadowOffsetY", 0);

        QColor shadowColor = CQJUtil::nameToColor(getStringProperty(js, "shadowColor", ""));

        canvas->fillShadowText(x + shadowX, y + shadowY, text, shadowBlur, shadowColor);
      }

      canvas->fillText(x, y, text);

      canvas->update();

      return CJValueP();
    }
  }
  else if (name == "strokeText") {
    initFont();

    initStroke();

    if (values.size() == 4) {
      std::string text = (values[1] ? values[1]->toString() : "");
      double      x    = (values[2] ? values[2]->toReal().getValue(0) : 0.0);
      double      y    = (values[3] ? values[3]->toReal().getValue(0) : 0.0);

      canvas->strokeText(x, y, text);

      canvas->update();

      return CJValueP();
    }
  }
  else if (name == "drawImage") {
    if (values.size() >= 4) {
      CJValueP imageVal = values[1];

      double sx = 0.0, sy = 0.0, sWidth = 0.0, sHeight = 0.0;
      double dx = 0.0, dy = 0.0, dWidth = 0.0, dHeight = 0.0;

      if      (values.size() == 4 || values.size() == 6) {
        dx = (values[2] ? values[2]->toReal().getValue(0) : 0.0);
        dy = (values[3] ? values[3]->toReal().getValue(0) : 0.0);

        if (values.size() == 6) {
          dWidth  = (values[4] ? values[4]->toReal().getValue(0) : 0.0);
          dHeight = (values[5] ? values[5]->toReal().getValue(0) : 0.0);
        }
      }
      else if (values.size() == 10) {
        sx      = (values[2] ? values[2]->toReal().getValue(0) : 0.0);
        sy      = (values[3] ? values[3]->toReal().getValue(0) : 0.0);
        sWidth  = (values[4] ? values[4]->toReal().getValue(0) : 0.0);
        sHeight = (values[5] ? values[5]->toReal().getValue(0) : 0.0);
        dx      = (values[6] ? values[6]->toReal().getValue(0) : 0.0);
        dy      = (values[7] ? values[7]->toReal().getValue(0) : 0.0);
        dWidth  = (values[8] ? values[8]->toReal().getValue(0) : 0.0);
        dHeight = (values[9] ? values[9]->toReal().getValue(0) : 0.0);
      }

      QImage qimage;

      if (imageVal && imageVal->type() == CJValue::Type::Object) {
        CJObjP imageObj = CJValue::cast<CJObj>(imageVal);

        if      (imageObj->objType()->name() == "Image") {
          CQJImage *qjimage = imageObj->castP<CQJImage>();

          qimage = qjimage->qimage();
        }
        else if (imageObj->objType()->name() == "ImageObj") {
          CQJImageObj *qjimage = imageObj->castP<CQJImageObj>();

          qimage = qjimage->qimage();
        }
      }

      double alpha = getRealProperty(js, "globalAlpha", 1.0);

      canvas->setOpacity(alpha);

      if (! qimage.isNull()) {
        if      (values.size() == 4)
          canvas->drawImage(qimage, dx, dy);
        else if (values.size() == 6)
          canvas->drawImage(qimage, dx, dy, dWidth, dHeight);
        else if (values.size() == 10)
          canvas->drawImage(qimage, sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight);
        else
          std::cerr << "Invalid number of args for '" << name << "'" << std::endl;
      }
      else
        std::cerr << "Invalid image for '" << name << "'" << std::endl;

      canvas->update();

      return CJValueP();
    }
  }
  else if (name == "beginPath") {
    canvas->beginPath();

    return CJValueP();
  }
  else if (name == "moveTo") {
    if (values.size() == 3) {
      double x = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double y = (values[2] ? values[2]->toReal().getValue(0) : 0);

      canvas->moveTo(x, y);

      return CJValueP();
    }
  }
  else if (name == "lineTo") {
    if (values.size() == 3) {
      double x = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double y = (values[2] ? values[2]->toReal().getValue(0) : 0);

      canvas->lineTo(x, y);

      return CJValueP();
    }
  }
  else if (name == "arc") {
    if (values.size() >= 6) {
      double x  = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double y  = (values[2] ? values[2]->toReal().getValue(0) : 0);
      double r  = (values[3] ? values[3]->toReal().getValue(0) : 0);
      double a1 = (values[4] ? values[4]->toReal().getValue(0) : 0);
      double a2 = (values[5] ? values[5]->toReal().getValue(0) : 0);

      bool counter = false;

      if (values.size() > 6)
        counter = (values[6] ? values[6]->toBoolean() : false);

      canvas->arc(x, y, r, a1, a2, counter);

      return CJValueP();
    }
  }
  else if (name == "closePath") {
    canvas->closePath();

    return CJValueP();
  }
  else if (name == "stroke") {
    initStroke();

    canvas->stroke();

    canvas->update();

    return CJValueP();
  }
  else if (name == "fill") {
    initFill();

    canvas->fill();

    canvas->update();

    return CJValueP();
  }
  else if (name == "save") {
    canvas->save();

    return CJValueP();
  }
  else if (name == "restore") {
    canvas->restore();

    return CJValueP();
  }
  else if (name == "rect") {
    if (values.size() == 5) {
      double x = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double y = (values[2] ? values[2]->toReal().getValue(0) : 0);
      double w = (values[3] ? values[3]->toReal().getValue(0) : 0);
      double h = (values[4] ? values[4]->toReal().getValue(0) : 0);

      canvas->rect(x, y, w, h);

      canvas->update();

      return CJValueP();
    }
  }
  else if (name == "clip") {
    canvas->clip();

    canvas->update();

    return CJValueP();
  }
  else if (name == "setTransform") {
    if (values.size() == 7) {
      double a = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double b = (values[2] ? values[2]->toReal().getValue(0) : 0);
      double c = (values[3] ? values[3]->toReal().getValue(0) : 0);
      double d = (values[4] ? values[4]->toReal().getValue(0) : 0);
      double e = (values[5] ? values[5]->toReal().getValue(0) : 0);
      double f = (values[6] ? values[6]->toReal().getValue(0) : 0);

      canvas->setTransform(a, b, c, d, e, f);

      return CJValueP();
    }
  }
  else if (name == "translate") {
    if (values.size() == 3) {
      double dx = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double dy = (values[1] ? values[2]->toReal().getValue(0) : 0);

      canvas->translate(dx, dy);

      return CJValueP();
    }
  }
  else if (name == "scale") {
    if (values.size() == 3) {
      double xs = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double ys = (values[2] ? values[2]->toReal().getValue(0) : 0);

      canvas->scale(xs, ys);

      return CJValueP();
    }
  }
  else if (name == "rotate") {
    if (values.size() == 2) {
      double a = (values[1] ? values[1]->toReal().getValue(0): 0);

      canvas->rotate(a);

      return CJValueP();
    }
  }
  else if (name == "createLinearGradient") {
    if (values.size() == 5) {
      double x1 = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double y1 = (values[2] ? values[2]->toReal().getValue(0) : 0);
      double x2 = (values[3] ? values[3]->toReal().getValue(0) : 0);
      double y2 = (values[4] ? values[4]->toReal().getValue(0) : 0);

      return CJValueP(new CQJCanvasLinearGradient(js, x1, y1, x2, y2));
    }
  }
  else if (name == "createRadialGradient") {
    if (values.size() == 7) {
      double x1 = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double y1 = (values[2] ? values[2]->toReal().getValue(0) : 0);
      double r1 = (values[3] ? values[3]->toReal().getValue(0) : 0);
      double x2 = (values[4] ? values[4]->toReal().getValue(0) : 0);
      double y2 = (values[5] ? values[5]->toReal().getValue(0) : 0);
      double r2 = (values[6] ? values[6]->toReal().getValue(0) : 0);

      return CJValueP(new CQJCanvasRadialGradient(js, x1, y1, r1, x2, y2, r2));
    }
  }
  else if (name == "createPattern") {
    if (values.size() == 3) {
      CJValueP    patternVal = values[1];
      std::string repeatStr  = (values[2] ? values[2]->toString() : "");

      CQJCanvasPattern::Repeat repeat = CQJCanvasPattern::Repeat::None;

      if      (repeatStr == "no-repeat")
        repeat = CQJCanvasPattern::Repeat::None;
      else if (repeatStr == "repeat-x")
        repeat = CQJCanvasPattern::Repeat::X;
      else if (repeatStr == "repeat-y")
        repeat = CQJCanvasPattern::Repeat::Y;

      if (patternVal->type() == CJValue::Type::Object) {
        CJObjP patternObj = CJValue::cast<CJObj>(patternVal);

        if      (patternObj->objType()->name() == "Image") {
          CQJImage *image = patternObj->castP<CQJImage>();

          return CJValueP(new CQJCanvasPattern(js, image->qimage(), repeat));
        }
      }
    }
  }
  else if (name == "createImageData") {
    if (values.size() == 3) {
      double w = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double h = (values[1] ? values[2]->toReal().getValue(0) : 0);

      return CJValueP(new CQJCanvasImageData(js, int(w), int(h)));
    }
  }
  else if (name == "getImageData") {
    if (values.size() == 5) {
      double x = (values[1] ? values[1]->toReal().getValue(0) : 0);
      double y = (values[2] ? values[2]->toReal().getValue(0) : 0);
      double w = (values[3] ? values[3]->toReal().getValue(0) : 0);
      double h = (values[4] ? values[4]->toReal().getValue(0) : 0);

      QImage image = canvas->getImageData(x, y, w, h);

      return CJValueP(new CQJCanvasImageData(js, image));
    }
  }
  else if (name == "putImageData") {
    if (values.size() == 4) {
      CJValueP imageVal = values[1];
      double   x        = (values[2] ? values[2]->toReal().getValue(0) : 0);
      double   y        = (values[3] ? values[3]->toReal().getValue(0) : 0);

      if (imageVal->type() == CJValue::Type::Object) {
        CJObjP imageObj = CJValue::cast<CJObj>(imageVal);

        if      (imageObj->objType()->name() == "Image") {
          CQJImage *image = imageObj->castP<CQJImage>();

          canvas->putImageData(image->qimage(), x, y);
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
  CQJCanvasWidget *canvas = canvasWidget();

  CJValueP fillStyle = getProperty(js_, "fillStyle");

  if (fillStyle->type() == CJValue::Type::Object) {
    CJObjP fillStyleObj = CJValue::cast<CJObj>(fillStyle);

    if      (fillStyleObj->objType()->name() == "CanvasLinearGradient") {
      CQJCanvasLinearGradient *lg = fillStyle->castP<CQJCanvasLinearGradient>();

      canvas->setFillGradient(lg->lg());
    }
    else if (fillStyleObj->objType()->name() == "CanvasRadialGradient") {
      CQJCanvasRadialGradient *rg = fillStyle->castP<CQJCanvasRadialGradient>();

      canvas->setFillGradient(rg->rg());
    }
    else if (fillStyleObj->objType()->name() == "CanvasPattern") {
      CQJCanvasPattern *pattern = fillStyle->castP<CQJCanvasPattern>();

      canvas->setFillPattern(pattern->qimage());
    }
  }
  else {
    std::string colorName = getStringProperty(js_, "fillStyle", "black");

    QColor c = CQJUtil::nameToColor(colorName);

    canvas->setFillColor(c);
  }

  double alpha = getRealProperty(js_, "globalAlpha", 1.0);

  canvas->setFillAlpha(alpha);
}

void
CQJCanvasContext2D::
initStroke()
{
  CQJCanvasWidget *canvas = canvasWidget();

  CJValueP strokeStyle = getProperty(js_, "strokeStyle");

  if (strokeStyle->type() == CJValue::Type::Object) {
    CJObjP strokeStyleObj = CJValue::cast<CJObj>(strokeStyle);

    if      (strokeStyleObj->objType()->name() == "CanvasLinearGradient") {
      CQJCanvasLinearGradient *lg = strokeStyle->castP<CQJCanvasLinearGradient>();

      canvas->setStrokeGradient(lg->lg());
    }
    else if (strokeStyleObj->objType()->name() == "CanvasRadialGradient") {
      CQJCanvasRadialGradient *rg = strokeStyle->castP<CQJCanvasRadialGradient>();

      canvas->setStrokeGradient(rg->rg());
    }
  }
  else {
    std::string colorName = getStringProperty(js_, "strokeStyle", "black");

    QColor c = CQJUtil::nameToColor(colorName);

    canvas->setStrokeColor(c);
  }

  double alpha = getRealProperty(js_, "globalAlpha", 1.0);
  double width = getRealProperty(js_, "lineWidth"  , 0);

  canvas->setStrokeAlpha(alpha);
  canvas->setStrokeWidth(width);

  std::string capName  = getStringProperty(js_, "lineCap" , "square");
  std::string joinName = getStringProperty(js_, "lineJoin", "miter" );

  if      (capName == "square")
    canvas->setStrokeCap(Qt::SquareCap);
  else if (capName == "round")
    canvas->setStrokeCap(Qt::RoundCap);
  else if (capName == "butt")
    canvas->setStrokeCap(Qt::FlatCap);

  if      (joinName == "bevel")
    canvas->setStrokeJoin(Qt::BevelJoin);
  else if (joinName == "round")
    canvas->setStrokeJoin(Qt::RoundJoin);
  else if (joinName == "miter")
    canvas->setStrokeJoin(Qt::MiterJoin);
}

void
CQJCanvasContext2D::
initFont()
{
  CQJCanvasWidget *canvas = canvasWidget();

  std::string textBaseline = getStringProperty(js_, "textBaseline", "alphabetic");
  std::string textAlign    = getStringProperty(js_, "textAlign"   , "left");

  if      (textBaseline == "top")
    canvas->setFontAlign(Qt::AlignTop);
  else if (textBaseline == "bottom")
    canvas->setFontAlign(Qt::AlignBottom);
  else if (textBaseline == "middle")
    canvas->setFontAlign(Qt::AlignVCenter);
  else if (textBaseline == "alphabetic")
    canvas->setFontAlign(Qt::AlignBaseline);
  else if (textBaseline == "hanging")
    canvas->setFontAlign(Qt::AlignTop);

  if      (textAlign == "start" || textAlign == "left")
    canvas->setFontAlign(canvas->fontAlign() | Qt::AlignLeft);
  else if (textAlign == "end" || textAlign == "right")
    canvas->setFontAlign(canvas->fontAlign() | Qt::AlignRight);
  else if (textAlign == "center")
    canvas->setFontAlign(canvas->fontAlign() | Qt::AlignHCenter);

  //---

  std::string fontStr = getStringProperty(js_, "font", "12px sans-serif");

  if (fontStr == lastFont_)
    return;

  lastFont_ = fontStr;

  //---

  QFont font;

  CStrParse parse(fontStr);

  //---

  bool found = true;

  while (found) {
    found = false;

    // font-style (normal, italic, oblique, initial, inherit)
    parse.skipSpace();

    if      (parse.isString("normal")) {
      parse.skipChars("normal");

      found = true;
    }
    else if (parse.isString("italic")) {
      parse.skipChars("italic");

      font.setItalic(true);

      found = true;
    }
    else if (parse.isString("oblique")) {
      parse.skipChars("oblique");

      font.setItalic(true);

      found = true;
    }
    else if (parse.isString("initial")) {
      parse.skipChars("initial");

      found = true;
    }
    else if (parse.isString("inherit")) {
      parse.skipChars("inherit");

      found = true;
    }

    // font-variant (small-caps)
    parse.skipSpace();

    if (parse.isString("small-caps")) {
      parse.skipChars("small-caps");

      found = true;
    }

    // font-weight
    if      (parse.isString("normal")) {
      parse.skipChars("normal");

      font.setWeight(QFont::Normal);

      found = true;
    }
    else if (parse.isString("bold")) {
      parse.skipChars("bold");

      font.setWeight(QFont::Bold);

      found = true;
    }
    // 100, 200, ...
  }

  //---

  // font-size/line-height
  parse.skipSpace();

  if (parse.isDigit()) {
    int fontSize = 12;

    parse.readInteger(&fontSize);

    if (parse.isString("px")) {
      parse.skipChars("px");

      font.setPixelSize(fontSize);
    }
    else if (parse.isString("pt")) {
      parse.skipChars("pt");

      font.setPointSize(fontSize);
    }
    else
      font.setPointSize(fontSize);

    //---

    if (parse.isChar('/')) {
      parse.skipChar();

      int lineHeight = 12;

      parse.readInteger(&lineHeight);

      if (parse.isString("px")) {
        parse.skipChars("px");
      }
      else if (parse.isString("pt")) {
        parse.skipChars("pt");
      }
    }
  }

  //---

  // font-family
  parse.skipSpace();

  std::string family;

  if (parse.readNonSpace(family)) {
    font.setFamily(family.c_str());
  }

  //---

  // caption|icon|menu|messagebox|small-caption|status-bar|initial|inherit

  canvas->setFont(font);
}

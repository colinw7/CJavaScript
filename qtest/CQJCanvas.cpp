#include <CQJCanvas.h>
#include <CQJCanvasContext2D.h>
#include <CQJImage.h>
#include <CQJCanvasWidget.h>
#include <CJavaScript.h>
#include <CStrParse.h>
#include <iostream>

CJObjTypeP CQJCanvasType::type_;

CJObjTypeP
CQJCanvasType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJCanvasType(js));

    js->addObjectType("Canvas", type_);
  }

  return type_;
}

CQJCanvasType::
CQJCanvasType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Canvas")
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

  CQJCanvas *canvas = values[0]->castP<CQJCanvas>();
  assert(canvas);

  return CJValueP();
}

//------

CQJCanvas::
CQJCanvas(CJavaScript *js, CQJCanvasWidget *canvas) :
 CQJObject(js, CQJCanvasType::instance(js)), canvas_(canvas)
{
  objType_->addObjFunction(js, "getContext", objType_);

  context2D_ = CJValueP(new CQJCanvasContext2D(js_, this));
}

CJValueP
CQJCanvas::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "offsetLeft") {
    return js->createNumberValue(0L);
  }
  else if (name == "offsetTop") {
    return js->createNumberValue(0L);
  }
  else if (name == "width") {
    return js->createNumberValue(long(canvas_->width()));
  }
  else if (name == "height") {
    return js->createNumberValue(long(canvas_->height()));
  }
  else
    return CQJObject::getProperty(js, name);
}

void
CQJCanvas::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  if      (name == "width") {
    //canvas_->setWidth(value->toInteger().getValue(1));
  }
  else if (name == "height") {
    //canvas_->setHeight(value->toInteger().getValue(1));
  }
  else
    CJObj::setProperty(js, name, value);
}

CJValueP
CQJCanvas::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if (name == "getContext") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      if (id == "2d")
        return context2D_;
    }

    return CJValueP();
  }
  else
    return CQJObject::execNameFn(js, name, values);
}

#include <CQJCanvas.h>
#include <CQJCanvasContext2D.h>
#include <CQJCanvasWidget.h>
#include <CQJavaScript.h>
#include <CQJImage.h>
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
CQJCanvas(CJavaScript *js) :
 CQJObject(js, CQJCanvasType::instance(js))
{
  objType_->addObjFunction(js, "getContext", objType_);

  context2D_ = CJValueP(new CQJCanvasContext2D(js_, this));

  updateSize();
}

void
CQJCanvas::
updateSize()
{
  CQJavaScript *qjs = CQJavaScript::instance();

  setRealProperty(js_, "width" , qjs->canvas()->width ());
  setRealProperty(js_, "height", qjs->canvas()->height());
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

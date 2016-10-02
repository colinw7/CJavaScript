#include <CQJCanvas.h>
#include <CQJCanvasContext2D.h>
#include <CQJSCanvas.h>
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
CQJCanvas(CQJavaScript *qjs) :
 CQJObject(qjs, CQJCanvasType::instance(qjs->js()))
{
  CJavaScript *js = qjs->js();

  objType_->addObjectFunction(js, "getContext");

  updateSize();
}

void
CQJCanvas::
updateSize()
{
  CJavaScript *js = qjs_->js();

  setRealProperty(js, "width" , qjs_->canvas()->width ());
  setRealProperty(js, "height", qjs_->canvas()->height());
}

CJValueP
CQJCanvas::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if (name == "getContext") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      if (id == "2d")
        return qjs_->jsCanvasContext2D();
    }

    return CJValueP();
  }
  else
    return CQJObject::execNameFn(js, name, values);
}

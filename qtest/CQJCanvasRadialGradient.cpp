#include <CQJCanvasRadialGradient.h>
#include <CQJavaScript.h>
#include <CQJUtil.h>

CJObjTypeP CQJCanvasRadialGradientType::type_;

CJObjTypeP
CQJCanvasRadialGradientType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CQJCanvasRadialGradientType(js));

  return type_;
}

CQJCanvasRadialGradientType::
CQJCanvasRadialGradientType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "CanvasRadialGradient")
{
}

//------

CQJCanvasRadialGradient::
CQJCanvasRadialGradient(CQJavaScript *qjs, double x1, double y1, double r1,
                        double x2, double y2, double r2) :
 CJObj(qjs->js(), CQJCanvasRadialGradientType::instance(qjs->js())), qjs_(qjs)
{
  CJavaScript *js = qjs->js();

  objType_->addObjectFunction(js, "addColorStop");

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

      QColor c = CQJUtil::nameToColor(colorName);

      rg_.setColorAt(r, c);
    }
  }

  return CJValueP();
}

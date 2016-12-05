#include <CQJCanvasLinearGradient.h>
#include <CQJUtil.h>

CJObjTypeP CQJCanvasLinearGradientType::type_;

CJObjTypeP
CQJCanvasLinearGradientType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CQJCanvasLinearGradientType(js));

  return type_;
}

CQJCanvasLinearGradientType::
CQJCanvasLinearGradientType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "CanvasLinearGradient")
{
}

//------

CQJCanvasLinearGradient::
CQJCanvasLinearGradient(CJavaScript *js, double x1, double y1, double x2, double y2) :
 CJObj(js, CQJCanvasLinearGradientType::instance(js))
{
  objType_->addObjFunction(js, "addColorStop", objType_);

  lg_ = QLinearGradient(x1, y1, x2, y2);
}

CJValueP
CQJCanvasLinearGradient::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if      (name == "addColorStop") {
    if (values.size() == 3) {
      double      r         = values[1]->toReal().getValue(0);
      std::string colorName = values[2]->toString();

      QColor c = CQJUtil::nameToColor(colorName);

      lg_.setColorAt(r, c);
    }
  }

  return CJValueP();
}

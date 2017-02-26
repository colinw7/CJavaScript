#include <CQJCanvasPattern.h>

CJObjTypeP CQJCanvasPatternType::type_;

CJObjTypeP
CQJCanvasPatternType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CQJCanvasPatternType(js));

  return type_;
}

CQJCanvasPatternType::
CQJCanvasPatternType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "CanvasPattern")
{
}

//------

CQJCanvasPattern::
CQJCanvasPattern(CJavaScript *js, const QImage &qimage, const Repeat &repeat) :
 CJObj(js, CQJCanvasPatternType::instance(js)), qimage_(qimage), repeat_(repeat)
{
}

CJValueP
CQJCanvasPattern::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

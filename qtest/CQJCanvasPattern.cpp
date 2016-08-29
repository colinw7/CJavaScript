#include <CQJCanvasPattern.h>
#include <CQJavaScript.h>

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
CQJCanvasPattern(CQJavaScript *qjs, const QImage &qimage, const Repeat &repeat) :
 CJObj(qjs->js(), CQJCanvasPatternType::instance(qjs->js())),
 qjs_(qjs), qimage_(qimage), repeat_(repeat)
{
}

CJValueP
CQJCanvasPattern::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

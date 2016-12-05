#include <CQJCanvasFontMetrics.h>

CJObjTypeP CQJCanvasFontMetricsType::type_;

CJObjTypeP
CQJCanvasFontMetricsType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CQJCanvasFontMetricsType(js));

  return type_;
}

CQJCanvasFontMetricsType::
CQJCanvasFontMetricsType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "CanvasFontMetrics")
{
}

//------

CQJCanvasFontMetrics::
CQJCanvasFontMetrics(CJavaScript *js, const QFont &font, const std::string &text) :
 CQJObject(js, CQJCanvasFontMetricsType::instance(js)),
 text_(text), font_(font), fm_(font)
{
  setRealProperty(js, "width", fm_.width(text.c_str()));
}

CJValueP
CQJCanvasFontMetrics::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

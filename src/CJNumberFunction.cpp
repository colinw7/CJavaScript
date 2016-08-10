#include <CJNumberFunction.h>
#include <CJavaScript.h>

CJNumberFunction::
CJNumberFunction(CJavaScript *js) :
 CJTypeFunction(js, "Number", CJNumberType::instance(js))
{
}

CJValueP
CJNumberFunction::
exec(CJavaScript *, const Values &)
{
  return CJValueP();
}

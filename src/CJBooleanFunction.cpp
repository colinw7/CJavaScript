#include <CJBooleanFunction.h>
#include <CJTrue.h>

CJBooleanFunction::
CJBooleanFunction(CJavaScript *js) :
 CJTypeFunction(js, "Boolean", CJTrueType::instance(js))
{
}

CJValueP
CJBooleanFunction::
exec(CJavaScript *, const Values &)
{
  return CJValueP();
}

#include <CJArrayFunction.h>
#include <CJArray.h>

CJArrayFunction::
CJArrayFunction(CJavaScript *js) :
 CJTypeFunction(js, "Array", CJArrayType::instance(js))
{
}

CJValueP
CJArrayFunction::
exec(CJavaScript *, const Values &)
{
  return CJValueP();
}

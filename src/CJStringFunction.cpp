#include <CJStringFunction.h>
#include <CJString.h>

CJStringFunction::
CJStringFunction(CJavaScript *js) :
 CJTypeFunction(js, "String", CJStringType::instance(js))
{
}

CJValueP
CJStringFunction::
exec(CJavaScript *, const Values &)
{
  return CJValueP();
}

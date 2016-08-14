#include <CJObjectFunction.h>
#include <CJavaScript.h>

CJObjectFunction::
CJObjectFunction(CJavaScript *js) :
 CJTypeFunction(js, "Object", CJObjectType::instance(js))
{
}

CJValueP
CJObjectFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() <= 1) {
    return CJValueP(new CJObject(js));
  }
  else {
    return CJValueP(values[1]->dup(js));
  }
}

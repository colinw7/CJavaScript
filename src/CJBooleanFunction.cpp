#include <CJBooleanFunction.h>
#include <CJavaScript.h>
#include <CJTrue.h>

CJBooleanFunction::
CJBooleanFunction(CJavaScript *js) :
 CJTypeFunction(js, "Boolean", CJTrueType::instance(js))
{
}

CJValueP
CJBooleanFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() <= 1)
    return js->createFalseValue();

  bool b = values[1]->toBoolean();

  if (b)
    return js->createTrueValue();
  else
    return js->createFalseValue();
}

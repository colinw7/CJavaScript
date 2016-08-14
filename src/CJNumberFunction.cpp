#include <CJNumberFunction.h>
#include <CJavaScript.h>

CJNumberFunction::
CJNumberFunction(CJavaScript *js) :
 CJTypeFunction(js, "Number", CJNumberType::instance(js))
{
}

CJValueP
CJNumberFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() <= 1)
    return js->createNumberValue(0L);
  else {
    double r = values[1]->toReal();

    return js->createNumberValue(r);
  }
}

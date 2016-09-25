#include <CJNumberFunction.h>
#include <CJavaScript.h>

CJNumberFunction::
CJNumberFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "Number", CJNumberType::instance(js))
{
}

CJValueP
CJNumberFunction::
exec(CJavaScript *js, const Values &values)
{
  CJNumber *number;

  if (values.size() <= 1)
    number = new CJNumber(js, 0L);
  else {
    double r = values[1]->toReal();

    number = new CJNumber(js, r);
  }

  number->setIsPrimitive(false);

  return CJValueP(number);
}

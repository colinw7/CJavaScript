#include <CJArrayFunction.h>
#include <CJArray.h>

CJArrayFunction::
CJArrayFunction(CJavaScript *js) :
 CJTypeFunction(js, "Array", CJArrayType::instance(js))
{
}

CJValueP
CJArrayFunction::
exec(CJavaScript *js, const Values &values)
{
  CJArray *array = 0;

  if      (values.size() == 1) {
    array = new CJArray(js);
  }
  else if (values.size() == 2) {
    long n = values[1]->toInteger();

    array = new CJArray(js, n);
  }
  else {
    array = new CJArray(js);

    for (uint i = 1; i < values.size(); ++i)
      array->addValue(values[i]);
  }

  return CJValueP(array);
}

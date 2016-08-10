#include <CJRealFunction.h>
#include <CJavaScript.h>

CJValueP
CJRealFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 1) {
    js->errorMsg("Wrong number of function values");
    return CJValueP();
  }

  if (! values[0])
    return CJValueP();

  double r = values[0]->toReal();

  double res = (*fn_)(r);

  return js->createNumberValue(res);
}

//------

CJValueP
CJReal2Function::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 2) {
    js->errorMsg("Wrong number of function values");
    return CJValueP();
  }

  double r1 = values[0]->toReal();
  double r2 = values[1]->toReal();

  double res = (*fn_)(r1, r2);

  return js->createNumberValue(res);
}

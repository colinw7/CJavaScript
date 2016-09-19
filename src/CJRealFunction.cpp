#include <CJRealFunction.h>
#include <CJavaScript.h>
#include <CJUtil.h>

CJValueP
CJMathFunction::
exec(CJavaScript *js, const Values &values)
{
  if      (fn_) {
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
  else if (name_ == "toString") {
    return js->createStringValue("[object Math]");
  }
  else
    return CJValueP();
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

//------

CJValueP
CJMinFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() == 0)
    return js->createNumberValue(CJUtil::getPosInf());

  double res = values[0]->toReal();

  for (uint i = 1; i < values.size(); ++i) {
    double r = values[i]->toReal();

    res = CJUtil::min(res, r);
  }

  return js->createNumberValue(res);
}

//------

CJValueP
CJMaxFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() == 0)
    return js->createNumberValue(CJUtil::getNegInf());

  double res = values[0]->toReal();

  for (uint i = 1; i < values.size(); ++i) {
    double r = values[i]->toReal();

    res = CJUtil::max(res, r);
  }

  return js->createNumberValue(res);
}

#include <CJRealFunction.h>
#include <CJavaScript.h>
#include <CJUtil.h>

CJValueP
CJMathFunction::
exec(CJavaScript *js, const Values &values)
{
  // values[0] is this

  if      (fn_) {
    if (values.size() < 2) {
      js->errorMsg("Wrong number of function values");
      return CJValueP();
    }

    if (! values[1])
      return CJValueP();

    double r = values[1]->toReal().getValue(0.0);

    double res = (*fn_)(r);

    return js->createNumberValue(res);
  }
  else if (name_ == "toString") {
    return js->createStringValue("[object Math]");
  }
  else
    return CJValueP();
}

std::string
CJMathFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJMathFunction::
print(std::ostream &os) const
{
  os << "double " << name_ << "(double)";
}

//------

CJValueP
CJReal2Function::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() < 2) {
    js->errorMsg("Wrong number of function values");
    return CJValueP();
  }

  double r1 = values[0]->toReal().getValue(0.0);
  double r2 = values[1]->toReal().getValue(0.0);

  double res = (*fn_)(r1, r2);

  return js->createNumberValue(res);
}

std::string
CJReal2Function::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJReal2Function::
print(std::ostream &os) const
{
  os << "double " << name_ << "(double, double)";
}

//------

CJMinFunction::
CJMinFunction(CJavaScript *js) :
 CJFunctionBase(js, "min", CJFunctionBase::Type::Min)
{
}

CJValueP
CJMinFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() <= 1)
    return js->createNumberValue(CJUtil::getPosInf());

  double res = values[1]->toReal().getValue(0.0);

  for (uint i = 2; i < values.size(); ++i) {
    double r = values[i]->toReal().getValue(0.0);

    res = CJUtil::min(res, r);
  }

  return js->createNumberValue(res);
}

std::string
CJMinFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJMinFunction::
print(std::ostream &os) const
{
  os << "double min(...)";
}

//------

CJMaxFunction::
CJMaxFunction(CJavaScript *js) :
 CJFunctionBase(js, "max", CJFunctionBase::Type::Max)
{
}

CJValueP
CJMaxFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() <= 1)
    return js->createNumberValue(CJUtil::getNegInf());

  double res = values[1]->toReal().getValue(0.0);

  for (uint i = 2; i < values.size(); ++i) {
    double r = values[i]->toReal().getValue(0.0);

    res = CJUtil::max(res, r);
  }

  return js->createNumberValue(res);
}

std::string
CJMaxFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJMaxFunction::
print(std::ostream &os) const
{
  os << "double max(...)";
}

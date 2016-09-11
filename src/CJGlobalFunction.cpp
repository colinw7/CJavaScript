#include <CJGlobalFunction.h>
#include <CJavaScript.h>
#include <CJUtil.h>

CJValueP
CJGlobalFunction::
exec(CJavaScript *js, const Values &values)
{
  if      (name_ == "eval") {
    if (values.size() >= 1) {
      if (values[0] && values[0]->type() == CJToken::Type::String) {
        std::string str = values[0]->toString();

        return js->interpString(str);
      }
      else
        return values[0];
    }
    else
      return js->createUndefinedValue();
  }
  else if (name_ == "isFinite") {
    if (values.size() < 1) {
      js->errorMsg("Wrong number of function values");
      return CJValueP();
    }

    double r = (values[0] ? values[0]->toReal() : 0.0);

    return js->createBoolValue(! CJUtil::isInf(r) && ! CJUtil::isNaN(r));
  }
  else if (name_ == "isNaN") {
    if (values.size() < 1) {
      js->errorMsg("Wrong number of function values");
      return CJValueP();
    }

    double r = (values[0] ? values[0]->toReal() : 0.0);

    return js->createBoolValue(CJUtil::isNaN(r));
  }
  else if (name_ == "parseInt") {
    if (values.size() < 1) {
      js->errorMsg("Wrong number of function values");
      return CJValueP();
    }

    std::string str = (values[0] ? values[0]->toString() : std::string());

    COptLong integer = CJString::parseInt(str, /*extraChars*/true);

    if (integer.isValid())
      return js->createNumberValue(integer.getValue());
    else
      return js->createNumberValue(CJUtil::getNaN());
  }
  else if (name_ == "parseFloat") {
    if (values.size() < 1) {
      js->errorMsg("Wrong number of function values");
      return CJValueP();
    }

    std::string str = (values[0] ? values[0]->toString() : std::string());

    COptReal real = CJString::parseFloat(str, /*extraChars*/true);

    if (real.isValid())
      return js->createNumberValue(real.getValue());
    else
      return js->createNumberValue(CJUtil::getNaN());
  }
  else
    return CJValueP();
}

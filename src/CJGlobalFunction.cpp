#include <CJGlobalFunction.h>
#include <CJavaScript.h>
#include <CJUtil.h>

CJValueP
CJGlobalFunction::
exec(CJavaScript *js, const Values &values)
{

  if      (name_ == "eval") {
    if (values.size() < 1) {
      js->errorMsg("Wrong number of function values");
      return CJValueP();
    }

    std::string str = values[0]->toString();

    CJavaScript js;

    js.loadString(str);

    return js.exec();
  }
  else if (name_ == "isNaN") {
    if (values.size() < 1) {
      js->errorMsg("Wrong number of function values");
      return CJValueP();
    }

    double r = values[0]->toReal();

    return js->createBoolValue(CJUtil::isNaN(r));
  }
  else if (name_ == "parseInt") {
    if (values.size() < 1) {
      js->errorMsg("Wrong number of function values");
      return CJValueP();
    }

    std::string s = values[0]->toString();

    return js->createNumberValue(CJString::parseInt(s));
  }
  else if (name_ == "parseFloat") {
    if (values.size() < 1) {
      js->errorMsg("Wrong number of function values");
      return CJValueP();
    }

    std::string s = values[0]->toString();

    return js->createNumberValue(CJString::parseFloat(s));
  }
  else
    return CJValueP();
}

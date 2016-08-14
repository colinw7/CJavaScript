#include <CJStringFunction.h>
#include <CJavaScript.h>
#include <CJString.h>

CJStringFunction::
CJStringFunction(CJavaScript *js) :
 CJTypeFunction(js, "String", CJStringType::instance(js))
{
}

CJValueP
CJStringFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() <= 1)
    return js->createStringValue("");

  std::string s = values[1]->toString();

  return js->createStringValue(s);
}

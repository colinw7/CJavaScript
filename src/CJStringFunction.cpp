#include <CJStringFunction.h>
#include <CJavaScript.h>
#include <CJString.h>

CJStringFunction::
CJStringFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "String", CJStringType::instance(js))
{
}

CJValueP
CJStringFunction::
exec(CJavaScript *js, const Values &values)
{
  CJString *str;

  if (values.size() <= 1)
    str = new CJString(js, "");
  else {
    std::string s = values[1]->toString();

    str = new CJString(js, s);
  }

  str->setIsBasic(false);

  return CJValueP(str);
}

#include <CJRegExpFunction.h>
#include <CJavaScript.h>
#include <CJRegExp.h>

CJRegExpFunction::
CJRegExpFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "RegExp", CJRegExpType::instance(js))
{
}

CJValueP
CJRegExpFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() <= 1)
    return js->createRegExpValue("");

  std::string s = values[1]->toString();

  std::string f;

  if (values.size() > 2)
    f = values[2]->toString();

  return js->createRegExpValue(s, f);
}

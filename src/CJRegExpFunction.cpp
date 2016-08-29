#include <CJRegExpFunction.h>
#include <CJavaScript.h>
#include <CJRegExp.h>

CJRegExpFunction::
CJRegExpFunction(CJavaScript *js) :
 CJTypeFunction(js, "RegExp", CJRegExpType::instance(js))
{
}

CJValueP
CJRegExpFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() < 1)
    return js->createRegExpValue("");

  std::string s = values[0]->toString();

  std::string f;

  if (values.size() > 1)
    f = values[1]->toString();

  return js->createRegExpValue(s, f);
}

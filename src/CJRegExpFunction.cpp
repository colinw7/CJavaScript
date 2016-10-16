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

  CJRegExpP regexp = js->createRegExpValue(s);

  if (values.size() > 2) {
    std::string flags = values[2]->toString();

    for (auto &f : flags) {
      if      (f == 'g') {
        regexp->setGlobalMatch(true);
      }
      else if (f == 'i') {
        regexp->setIgnoreCase(true);
      }
      else if (f == 'm') {
      }
      else if (f == 'u') {
      }
      else if (f == 'y') {
      }
    }
  }

  return regexp;
}

CJValueP
CJRegExpFunction::
execNew(CJavaScript *js, const Values &values)
{
  return exec(js, values);
}

std::string
CJRegExpFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJRegExpFunction::
print(std::ostream &os) const
{
  os << "[Function: RegExp]";
}

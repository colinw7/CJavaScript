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
  CJStringP str;

  if (values.size() <= 1)
    str = js->createStringValue("");
  else {
    std::string s = values[1]->toString();

    str = js->createStringValue(s);
  }

  str->setIsPrimitive(true);

  return str;
}

CJValueP
CJStringFunction::
execNew(CJavaScript *js, const Values &values)
{
  CJStringP str;

  if (values.size() <= 1)
    str = js->createStringValue("");
  else {
    std::string s = values[1]->toString();

    str = js->createStringValue(s);
  }

  str->setIsPrimitive(false);

  return str;
}

std::string
CJStringFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJStringFunction::
print(std::ostream &os) const
{
  os << "[Function: String]";
}

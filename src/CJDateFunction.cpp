#include <CJDateFunction.h>
#include <CJavaScript.h>
#include <CJDate.h>

CJDateFunction::
CJDateFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "Date", CJDateType::instance(js))
{
}

CJValueP
CJDateFunction::
exec(CJavaScript *js, const Values &)
{
  std::string res = CJDate::getTimeString();

  return js->createStringValue(res);
}

CJValueP
CJDateFunction::
execNew(CJavaScript *js, const Values &values)
{
  return CJValueP(new CJDate(js, values));
}

std::string
CJDateFunction::
toString() const
{
  return "[Function: Date]";
}

void
CJDateFunction::
print(std::ostream &os) const
{
  os << "[Function: Date]";
}

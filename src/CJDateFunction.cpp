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

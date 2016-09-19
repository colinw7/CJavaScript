#include <CJCallFunction.h>
#include <CJavaScript.h>

CJCallFunction::
CJCallFunction(CJavaScript *js, CJFunctionBaseP function) :
 CJFunctionBase(js, "call", CJFunctionBase::Type::Call), function_(function)
{
}

CJValueP
CJCallFunction::
getProperty(CJavaScript *js, const std::string &name) const
{
  return CJFunctionBase::getProperty(js, name);
}

void
CJCallFunction::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJFunctionBase::setProperty(js, name, value);
}

CJValueP
CJCallFunction::
exec(CJavaScript *, const Values &)
{
  return CJValueP();
}

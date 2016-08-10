#include <CJObject.h>
#include <CJObjectType.h>
#include <CJTypeValue.h>

const std::string &
CJObject::
name() const
{
  return type_->name();
}

void
CJObject::
addVariable(CJavaScript *js, const std::string &name)
{
  setProperty(name, CJValueP(new CJTypeValue(js, type(), name)));
}

CJValueP
CJObject::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  assert(false);
}

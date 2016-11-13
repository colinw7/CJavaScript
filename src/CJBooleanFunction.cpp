#include <CJBooleanFunction.h>
#include <CJavaScript.h>
#include <CJTrue.h>

CJBooleanFunction::
CJBooleanFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "Boolean", CJBooleanType::instance(js))
{
}

CJBooleanFunction::
CJBooleanFunction(const CJBooleanFunction &fn) :
 CJObjTypeFunction(fn)
{
}

CJValueP
CJBooleanFunction::
exec(CJavaScript *js, const Values &values)
{
  CJValueP bobj;

  if (values.size() <= 1)
    bobj = js->createBoolValue();
  else {
    bool b = values[1]->toBoolean();

    bobj = js->createBoolValue(b);
  }

  return bobj;
}

CJValueP
CJBooleanFunction::
execNew(CJavaScript *js, const Values &values)
{
  CJBooleanP bobj;

  if (values.size() <= 1)
    bobj = js->createBoolObject();
  else {
    bool b = values[1]->toBoolean();

    bobj = js->createBoolObject(b);
  }

  return bobj;
}

std::string
CJBooleanFunction::
toString() const
{
  return "[Function: Boolean]";
}

void
CJBooleanFunction::
print(std::ostream &os) const
{
  os << "[Function: Boolean]";
}

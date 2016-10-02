#include <CJBooleanFunction.h>
#include <CJavaScript.h>
#include <CJTrue.h>

CJBooleanFunction::
CJBooleanFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "Boolean", CJBooleanType::instance(js))
{
}

CJValueP
CJBooleanFunction::
exec(CJavaScript *js, const Values &values)
{
  bool b = false;

  if (values.size() > 1)
    b = values[1]->toBoolean();

  return js->createBoolValue(b);
}

CJValueP
CJBooleanFunction::
execNew(CJavaScript *js, const Values &values)
{
  CJBoolean *bobj;

  if (values.size() <= 1)
    bobj = new CJBoolean(js);
  else {
    bool b = values[1]->toBoolean();

    bobj = new CJBoolean(js, b);
  }

  return CJValueP(bobj);
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

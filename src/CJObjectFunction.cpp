#include <CJObjectFunction.h>
#include <CJavaScript.h>

CJObjectFunction::
CJObjectFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "Object", CJObjectType::instance(js))
{
}

CJValueP
CJObjectFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() <= 1 || ! values[1])
    return js->createObject();

  return CJValueP(values[1]->dup(js));
}

CJValueP
CJObjectFunction::
execNew(CJavaScript *js, const Values &values)
{
  return exec(js, values);
}

std::string
CJObjectFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJObjectFunction::
print(std::ostream &os) const
{
  os << "[Function: Object]";
}

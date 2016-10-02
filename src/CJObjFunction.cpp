#include <CJObjFunction.h>
#include <CJavaScript.h>

CJValueP
CJObjFunction::
exec(CJavaScript *js, const Values &values)
{
  assert(values.size() > 0);

  CJValueP value;

  if (! values[0])
    return value;

  CJValueP ovalue = values[0];

  if (ovalue->type() == CJToken::Type::Object) {
    value = CJValue::cast<CJObj>(ovalue)->execNameFn(js, name(), values);
  }
  else {
    js->errorMsg("Invalid object function type");
  }

  return value;
}

std::string
CJObjFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJObjFunction::
print(std::ostream &os) const
{
  os << "object fn";
}

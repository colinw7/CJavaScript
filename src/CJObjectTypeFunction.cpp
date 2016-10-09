#include <CJObjectTypeFunction.h>
#include <CJavaScript.h>
#include <CJString.h>
#include <CJArray.h>

CJObjectTypeFunction::
CJObjectTypeFunction(CJavaScript *js, const std::string &name, CJObjTypeP type) :
 CJFunctionBase(js, name, CJFunctionBase::Type::Object), type_(type)
{
}

// call type function for object
CJValueP
CJObjectTypeFunction::
exec(CJavaScript *js, const Values &values)
{
  assert(values.size() > 0);

  CJValueP value;

  if (! values[0])
    return value;

  CJValueP ovalue = values[0];

  CJObjTypeP objType = type_;

  if (! objType) {
    if (ovalue->isObject()) {
      CJObjP obj = CJValue::cast<CJObj>(ovalue);

      if (! obj)
        return value;

      objType = obj->objType();
    }
    else {
      js->errorMsg("Invalid object function type");
    }
  }

  value = objType->exec(js, name(), values);

  return value;
}

std::string
CJObjectTypeFunction::
toString() const
{
  return "function " + name_ + "() { [native code] }";
}

void
CJObjectTypeFunction::
print(std::ostream &os) const
{
  os << "[Function " + name_ + "]";
}

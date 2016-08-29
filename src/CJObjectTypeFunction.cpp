#include <CJObjectTypeFunction.h>
#include <CJavaScript.h>
#include <CJObject.h>
#include <CJString.h>
#include <CJArray.h>

CJObjectTypeFunction::
CJObjectTypeFunction(CJavaScript *js, const std::string &name, CJObjTypeP type) :
 CJFunction(js, name, CJFunction::Type::Object), type_(std::move(type))
{
}

CJValueP
CJObjectTypeFunction::
exec(CJavaScript *js, const Values &values)
{
  assert(values.size() > 0);

  CJValueP value;

  if (! values[0])
    return value;

  CJValueP ovalue = values[0];

  if      (ovalue->type() == CJToken::Type::Object) {
    value = ovalue->cast<CJObj>()->execNameFn(js, name(), values);
  }
  else if (ovalue->isObject()) {
    CJObj *obj = ovalue->cast<CJObj>();

    if (! obj)
      return value;

    CJObjTypeP objType = obj->type();

    value = objType->exec(js, name(), values);
  }
  else {
    js->errorMsg("Invalid object function type");
  }

  return value;
}

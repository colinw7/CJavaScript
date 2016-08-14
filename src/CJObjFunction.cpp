#include <CJObjFunction.h>
#include <CJObject.h>
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
    value = ovalue->cast<CJObj>()->execNameFn(js, name(), values);
  }
  else {
    js->errorMsg("Invalid object function type");
  }

  return value;
}

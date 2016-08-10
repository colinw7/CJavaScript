#include <CJTypeFunction.h>
#include <CJavaScript.h>
#include <CJObject.h>
#include <CJString.h>
#include <CJArray.h>

CJValueP
CJTypeFunction::
exec(CJavaScript *js, const Values &values)
{
  assert(values.size() > 0);

  CJValueP value;

  if (! values[0])
    return value;

  CJValueP ovalue = values[0];

  if      (ovalue->type() == CJToken::Type::Object) {
    value = ovalue->cast<CJObject>()->execNameFn(js, name(), values);
  }
  else if (ovalue->type() == CJToken::Type::String) {
    CJObjectTypeP stringType = CJStringType::instance(js);

    value = stringType->exec(js, name(), values);
  }
  else if (ovalue->type() == CJToken::Type::Array) {
    CJObjectTypeP arrayType = CJArrayType::instance(js);

    value = arrayType->exec(js, name(), values);
  }
  else {
    js->errorMsg("Invalid object function type");
  }

  return value;
}

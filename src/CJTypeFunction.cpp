#include <CJTypeFunction.h>
#include <CJavaScript.h>
#include <CJObject.h>
#include <CJString.h>
#include <CJArray.h>

CJTypeFunction::
CJTypeFunction(CJavaScript *js, const std::string &name, CJObjTypeP type) :
 CJFunction(js, name, CJFunction::Type::Type), type_(type)
{
}

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
    value = ovalue->cast<CJObj>()->execNameFn(js, name(), values);
  }
  else if (ovalue->type() == CJToken::Type::String) {
    CJObjTypeP stringType = CJStringType::instance(js);

    value = stringType->exec(js, name(), values);
  }
  else if (ovalue->type() == CJToken::Type::Array) {
    CJObjTypeP arrayType = CJArrayType::instance(js);

    value = arrayType->exec(js, name(), values);
  }
  else {
    js->errorMsg("Invalid object function type");
  }

  return value;
}

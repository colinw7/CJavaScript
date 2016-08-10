#include <CJObjectFunction.h>
#include <CJArray.h>
#include <CJObject.h>
#include <CJavaScript.h>

CJValueP
CJObjectFunction::
exec(CJavaScript *js, const Values &values)
{
  assert(values.size() > 0);

  CJValueP value;

  if (! values[0])
    return value;

  CJValueP ovalue = values[0];

  if (name_ == "getOwnPropertyNames") {
    CJArrayP array(new CJArray(js));

    if      (ovalue->type() == CJToken::Type::Object) {
      CJObjectTypeP valueType = ovalue->cast<CJObject>()->valueType();

      auto names = valueType->getPropertyNames();

      for (const auto &n : names) {
        array->addValue(js->createStringValue(n));
      }
    }
    else if (ovalue->type() == CJToken::Type::Dictionary) {
      if (values.size() > 1) {
        ovalue = values[1];

        if (ovalue->type() != CJToken::Type::Dictionary) {
          js->errorMsg("Invalid object function type");
          return value;
        }
      }

      CJDictionary *dict = ovalue->cast<CJObject>()->cast<CJDictionary>();

      for (auto &kv : dict->keyValues()) {
        std::string key = kv.first;

        array->addValue(js->createStringValue(key));
      }
    }
    else {
      js->errorMsg("Invalid object function type");
    }

    value = std::static_pointer_cast<CJValue>(array);
  }
  else {
    if (ovalue->type() == CJToken::Type::Object) {
      value = ovalue->cast<CJObject>()->execNameFn(js, name(), values);
    }
    else {
      js->errorMsg("Invalid object function type");
    }
  }

  return value;
}

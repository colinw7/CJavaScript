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
getProperty(CJavaScript *js, const std::string &name) const
{
  if (type_->hasProperty(js, name))
    return type_->getProperty(js, name);
  else
    return CJFunction::getProperty(js, name);
}

void
CJTypeFunction::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  if (type_->hasProperty(js, name))
    type_->setProperty(js, name, value);
  else
    CJFunction::setProperty(js, name, value);
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
  else if (ovalue->type() == CJToken::Type::Date) {
    CJObjTypeP dateType = CJDateType::instance(js);

    value = dateType->exec(js, name(), values);
  }
  else if (ovalue->type() == CJToken::Type::Function) {
    CJTypeFunctionP typeFn = std::static_pointer_cast<CJTypeFunction>(ovalue);

    CJObjTypeP objType = typeFn->objectType();

    value = objType->exec(js, name(), values);
  }
  else if (ovalue->type() == CJToken::Type::Dictionary) {
    if (name_ == "getOwnPropertyNames") {
      CJArrayP array(new CJArray(js));

      CJDictionary *dict = ovalue->cast<CJDictionary>();

      for (auto &kv : dict->keyValues()) {
        std::string key = kv.first;

        array->addValue(js->createStringValue(key));
      }

      value = std::static_pointer_cast<CJValue>(array);
    }
    else {
      js->errorMsg("Invalid dictionary type function");
    }
  }
  else {
    js->errorMsg("Invalid object function type");
  }

  return value;
}

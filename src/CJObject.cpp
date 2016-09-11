#include <CJObject.h>
#include <CJObj.h>
#include <CJTypeFunction.h>
#include <CJavaScript.h>

CJObjTypeP CJObjectType::type_;

CJObjTypeP
CJObjectType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJObjectType(js));

    js->addObjectType("object", type_);

    js->addTypeObject(CJToken::Type::Object, type_);
  }

  return type_;
}

CJObjectType::
CJObjectType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "object")
{
  addTypeFunction(js, "getOwnPropertyNames");
  addTypeFunction(js, "defineProperty");
}

CJValueP
CJObjectType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if      (name == "getOwnPropertyNames") {
    if (values.size() <= 1) {
      js->errorMsg("Missing value for getOwnPropertyNames");
      return CJValueP();
    }

    CJValueP ovalue = values[1];

    if (! ovalue)
      return CJValueP();

    CJArrayP array(new CJArray(js));

    if      (ovalue->type() == CJToken::Type::Object) {
      CJObjTypeP valueType = ovalue->cast<CJObj>()->valueType();

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
          return CJValueP();
        }
      }

      CJDictionary *dict = ovalue->cast<CJObj>()->cast<CJDictionary>();

      for (auto &kv : dict->keyValues()) {
        std::string key = kv.first;

        array->addValue(js->createStringValue(key));
      }
    }
    else if (ovalue->type() == CJToken::Type::Function) {
      CJFunctionP fn = std::static_pointer_cast<CJFunction>(ovalue);

      CJObjTypeP valueType;

      if (fn->type() == CJFunction::Type::Type) {
        CJTypeFunctionP typeFn = std::static_pointer_cast<CJTypeFunction>(ovalue);

        valueType = typeFn->objectType();
      }

      if (valueType) {
        auto names = valueType->getTypePropertyNames();

        for (const auto &n : names) {
          array->addValue(js->createStringValue(n));
        }
      }
      else {
        js->errorMsg("Invalid object function type");
      }
    }
    else {
      js->errorMsg("Invalid object function type");
    }

    CJValueP value = std::static_pointer_cast<CJValue>(array);

    return value;
  }
  else if (name == "defineProperty") {
    if (values.size() != 4)
      return CJValueP();

    CJValueP obj   = values[1];
    CJValueP prop  = values[2];
    CJValueP desc = values[3];

    if (desc->type() == CJToken::Type::Dictionary) {
      CJDictionaryP dict = std::static_pointer_cast<CJDictionary>(desc);

      CJValueP value    = dict->getProperty(js, "value");
      CJValueP writable = dict->getProperty(js, "writable");

      if      (obj->hasIndex()) {
        int ind = prop->toInteger();

        if (value)
          obj->setIndexValue(ind, value);

        if (writable)
          obj->setReadOnlyIndex(ind, writable->toBoolean());
      }
      else if (obj->hasProperty()) {
        std::string ind = prop->toString();

        if (value)
          obj->setPropertyValue(ind, value);

        if (writable)
          obj->setReadOnlyProperty(ind, writable->toBoolean());
      }
      else {
        js->errorMsg("Invalid object for defineProperty");
      }
    }
    else {
      js->errorMsg("Invalid descriptor for defineProperty");
    }

    return CJValueP();
  }
  else
    return CJValueP();
}

//------

CJObject::
CJObject(CJavaScript *js) :
 CJObj(js, CJObjectType::instance(js))
{
}

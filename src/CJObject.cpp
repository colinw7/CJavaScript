#include <CJObject.h>
#include <CJObj.h>
#include <CJTypeFunction.h>
#include <CJGetterSetter.h>
#include <CJFunction.h>
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
  addTypeFunction(js, "toString");
  addTypeFunction(js, "getOwnPropertyNames");
  addTypeFunction(js, "defineProperty");
}

CJValueP
CJObjectType::
execType(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  // values[0] is CJObjectFunction

  //---

  // type functions
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
    else if (ovalue->isFunction()) {
      CJFunctionBaseP fn = std::static_pointer_cast<CJFunctionBase>(ovalue);

      CJObjTypeP valueType;

      if (fn->type() == CJFunctionBase::Type::ObjType) {
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

    CJValueP obj  = values[1];
    CJValueP prop = values[2];
    CJValueP desc = values[3];

    if (desc->type() == CJToken::Type::Dictionary) {
      CJDictionaryP dict = std::static_pointer_cast<CJDictionary>(desc);

      CJValueP value        = dict->getProperty(js, "value");
      CJValueP writable     = dict->getProperty(js, "writable");
      CJValueP enumerable   = dict->getProperty(js, "enumerable");
      CJValueP configurable = dict->getProperty(js, "configurable");
      CJValueP getter       = dict->getProperty(js, "get");
      CJValueP setter       = dict->getProperty(js, "set");

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

        if (obj->type() == CJToken::Type::Dictionary && (getter || setter)) {
          CJDictionaryP dict = std::static_pointer_cast<CJDictionary>(obj);

          CJGetterSetterP gs(new CJGetterSetter(js));

          if (getter && getter->isFunction() &&
              std::static_pointer_cast<CJFunctionBase>(getter)->type() ==
                CJFunctionBase::Type::User)
            gs->setGetter(std::static_pointer_cast<CJFunction>(getter));

          if (setter && setter->isFunction() &&
              std::static_pointer_cast<CJFunctionBase>(setter)->type() ==
                CJFunctionBase::Type::User)
            gs->setGetter(std::static_pointer_cast<CJFunction>(setter));

          obj->setPropertyValue(ind, gs);
        }
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
  else if (name == "toString") {
    return js->createStringValue("[object Function]");
  }
  else
    return CJValueP();
}

CJValueP
CJObjectType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJObject *obj = values[0]->cast<CJObject>();
  assert(obj);

  js->errorMsg("Invalid array object function " + name);

  return CJValueP();
}

//------

CJObject::
CJObject(CJavaScript *js) :
 CJObj(js, CJObjectType::instance(js))
{
}

std::string
CJObject::
toString() const
{
  std::ostringstream ss;

  if (typeName_ != "")
    ss << typeName_ << " ";

  ss << "{";

  int i = 0;

  for (const auto &kv : keyValues_) {
    if (i > 0)
      ss << ",";

    ss << " " << kv.first << ": " << *kv.second;

    ++i;
  }

  ss << " }";

  return ss.str();
}

void
CJObject::
print(std::ostream &os) const
{
  os << toString();
}

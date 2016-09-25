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
  addTypeFunction(js, "keys");
  addTypeFunction(js, "defineProperty");
  addTypeFunction(js, "defineProperties");

  addObjectFunction(js, "propertyIsEnumerable"); // TODO: move to base class
  addObjectFunction(js, "hasOwnProperty"); // TODO: move to base class
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

    return array;
  }
  else if (name == "keys") {
    if (values.size() <= 1) {
      js->errorMsg("Missing value for keys");
      return CJValueP();
    }

    CJValueP ovalue = values[1];

    if (! ovalue)
      return CJValueP();

    CJArrayP array(new CJArray(js));

    return array;
  }
  else if (name == "defineProperty") {
    if (values.size() != 4)
      return CJValueP();

    CJValueP obj  = values[1];
    CJValueP prop = values[2];
    CJValueP desc = values[3];

    if (desc->type() == CJToken::Type::Object ||
        desc->type() == CJToken::Type::Dictionary) {
      CJValueP value, writable, enumerable, configurable, getter, setter;

      if (desc->type() == CJToken::Type::Object) {
        CJObjectP descObj = std::static_pointer_cast<CJObject>(desc);

        value        = descObj->getProperty(js, "value");
        writable     = descObj->getProperty(js, "writable");
        enumerable   = descObj->getProperty(js, "enumerable");
        configurable = descObj->getProperty(js, "configurable");
        getter       = descObj->getProperty(js, "get");
        setter       = descObj->getProperty(js, "set");
      }
      else {
        CJDictionaryP descDict = std::static_pointer_cast<CJDictionary>(desc);

        value        = descDict->getProperty(js, "value");
        writable     = descDict->getProperty(js, "writable");
        enumerable   = descDict->getProperty(js, "enumerable");
        configurable = descDict->getProperty(js, "configurable");
        getter       = descDict->getProperty(js, "get");
        setter       = descDict->getProperty(js, "set");
      }

      if      (obj->hasIndex()) {
        int ind = prop->toInteger();

        if (value)
          obj->setIndexValue(ind, value);

        if (writable)
          obj->setWritableIndex(ind, writable->toBoolean());

        if (enumerable)
          obj->setEnumerableIndex(ind, enumerable->toBoolean());
      }
      else if (obj->hasProperty()) {
        CJDictionaryP objDict;

        if (obj->isDictionary())
          objDict = std::static_pointer_cast<CJDictionary>(obj);

        std::string ind = prop->toString();

        if (value)
          obj->setPropertyValue(ind, value);

        if (objDict) {
          if (writable)
            objDict->setWritableProperty(ind, writable->toBoolean());

          if (enumerable)
            objDict->setEnumerableProperty(ind, enumerable->toBoolean());
        }

        if (obj->isDictionary() && (getter || setter)) {
          CJGetterSetterP gs(new CJGetterSetter(js));

          if (getter && getter->isFunction() &&
              std::static_pointer_cast<CJFunctionBase>(getter)->type() ==
                CJFunctionBase::Type::User)
            gs->setGetter(std::static_pointer_cast<CJFunction>(getter));

          if (setter && setter->isFunction() &&
              std::static_pointer_cast<CJFunctionBase>(setter)->type() ==
                CJFunctionBase::Type::User)
            gs->setSetter(std::static_pointer_cast<CJFunction>(setter));

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
  else if (name == "defineProperties") {
    if (values.size() != 3)
      return CJValueP();

    CJValueP obj  = values[1];
    CJValueP prop = values[2];

    if (obj->type() != CJToken::Type::Object) {
      js->throwTypeError(obj, "Invalid defineProperties argument type");
      return CJValueP();
    }

    CJObjectP objObj = std::static_pointer_cast<CJObject>(obj);

    if (prop->type() != CJToken::Type::Object) {
      js->throwTypeError(prop, "Invalid defineProperties argument type");
      return CJValueP();
    }

    CJObjectP propObj = std::static_pointer_cast<CJObject>(prop);

    for (const auto &name : propObj->propertyNames()) {
      CJValueP desc = propObj->getProperty(js, name);

      if (! propObj->isEnumerableProperty(name))
        continue;

      if (desc->type() != CJToken::Type::Object) {
        js->throwTypeError(desc, "Invalid defineProperties argument type");
        continue;
      }

      CJObjectP descObj = std::static_pointer_cast<CJObject>(desc);

      //---

      CJValueP value        = descObj->getProperty(js, "value");
      CJValueP writable     = descObj->getProperty(js, "writable");
      CJValueP enumerable   = descObj->getProperty(js, "enumerable");
      CJValueP configurable = descObj->getProperty(js, "configurable");
      CJValueP getter       = descObj->getProperty(js, "get");
      CJValueP setter       = descObj->getProperty(js, "set");

      if (getter && ! getter->isFunction()) {
        js->throwTypeError(desc, "Invalid get property type");
        continue;
      }

      if (setter && ! setter->isFunction()) {
        js->throwTypeError(desc, "Invalid set property type");
        continue;
      }

      //---

      if (value)
        objObj->setProperty(js, name, value);

      if (writable)
        objObj->setWritableProperty(name, writable->toBoolean());

      if (enumerable)
        objObj->setEnumerableProperty(name, enumerable->toBoolean());

      if (getter || setter) {
        CJGetterSetterP gs(new CJGetterSetter(js));

        if (getter && getter->isFunction() &&
            std::static_pointer_cast<CJFunctionBase>(getter)->type() ==
              CJFunctionBase::Type::User)
          gs->setGetter(std::static_pointer_cast<CJFunction>(getter));

        if (setter && setter->isFunction() &&
            std::static_pointer_cast<CJFunctionBase>(setter)->type() ==
              CJFunctionBase::Type::User)
          gs->setSetter(std::static_pointer_cast<CJFunction>(setter));

        objObj->setProperty(js, name, gs);
      }
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

  // object functions
  if      (name == "propertyIsEnumerable") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string ind = values[1]->toString();

    bool b = false;

    if (obj->hasProperty(js, ind))
      b = obj->isEnumerableProperty(ind);

    return js->createBoolValue(b);
  }
  else if (name == "hasOwnProperty") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    return js->createBoolValue(js->hasIndexValue(values[0], values[1]));
  }
  else {
    js->errorMsg("Invalid object instance function " + name);
  }

  return CJValueP();
}

//------

CJObject::
CJObject(CJavaScript *js) :
 CJObj(js, CJObjectType::instance(js))
{
}

CJValueP
CJObject::
getProperty(CJavaScript *js, const std::string &key) const
{
  CJValueP propVal = CJObj::getProperty(js, key);

  if (propVal && propVal->type() == CJToken::Type::GetterSetter) {
    CJGetterSetterP gs = std::static_pointer_cast<CJGetterSetter>(propVal);

    CJObject *th = const_cast<CJObject *>(this);

    CJObjectP obj = std::static_pointer_cast<CJObject>(th->shared_from_this());

    js->pushThis(obj);

    propVal = gs->getValue();

    js->popThis();
  }

  return propVal;
}

void
CJObject::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJValueP propVal = CJObj::getProperty(js, key);

  if (propVal && propVal->type() == CJToken::Type::GetterSetter) {
    CJGetterSetterP gs = std::static_pointer_cast<CJGetterSetter>(propVal);

    CJObjectP obj = std::static_pointer_cast<CJObject>(shared_from_this());

    js->pushThis(obj);

    gs->setValue(value);

    js->popThis();

    return;
  }

  CJObj::setProperty(js, key, value);
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

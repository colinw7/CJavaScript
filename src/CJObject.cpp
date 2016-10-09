#include <CJObject.h>
#include <CJObj.h>
#include <CJTypeFunction.h>
#include <CJObjTypeFunction.h>
#include <CJGetterSetter.h>
#include <CJFunction.h>
#include <CJavaScript.h>

CJObjectTypeP CJObjectType::type_;

CJObjectTypeP
CJObjectType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = std::make_shared<CJObjectType>(js);

    type_->init();

    js->addObjectType(type_->name(), type_);

    js->addTypeObject(CJToken::Type::Object, type_);
  }

  return type_;
}

CJObjectType::
CJObjectType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Object")
{
}

void
CJObjectType::
init()
{
  addTypeFunction(js_, "assign"                  , type_);
  addTypeFunction(js_, "create"                  , type_);
  addTypeFunction(js_, "defineProperties"        , type_);
  addTypeFunction(js_, "defineProperty"          , type_);
  addTypeFunction(js_, "freeze"                  , type_);
  addTypeFunction(js_, "getOwnPropertyDescriptor", type_);
  addTypeFunction(js_, "getOwnPropertyNames"     , type_);
  addTypeFunction(js_, "getPrototypeOf"          , type_);
  addTypeFunction(js_, "isExtensible"            , type_);
  addTypeFunction(js_, "isFrozen"                , type_);
  addTypeFunction(js_, "isSealed"                , type_);
  addTypeFunction(js_, "keys"                    , type_);
  addTypeFunction(js_, "preventExtensions"       , type_);
  addTypeFunction(js_, "seal"                    , type_);
  addTypeFunction(js_, "toString"                , type_);

  addObjectFunction(js_, "hasOwnProperty"      , type_); // TODO: move to base class
  addObjectFunction(js_, "isPrototypeOf"       , type_);
  addObjectFunction(js_, "propertyIsEnumerable", type_); // TODO: move to base class
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
  if      (name == "assign") {
    // Object.assign(target, ...sources)
    if (values.size() < 2)
      return CJValueP();

    CJValueP targetValue = values[1];

    CJObjectP targetObj;

    if (targetValue->isObject())
      targetObj = CJValue::cast<CJObject>(targetValue);

    for (std::size_t i = 2; i < values.size(); ++i) {
      CJValueP sourceValue = values[i];

      if (sourceValue->isObject()) {
        CJObjectP sourceObj = CJValue::cast<CJObject>(sourceValue);

        for (const auto &name : sourceObj->propertyNames()) {
          if (! sourceObj->isEnumerableProperty(name))
            continue;

          targetObj->setProperty(js, name, sourceObj->getProperty(js, name));
        }
      }
    }

    return targetValue;
  }
  else if (name == "create") {
    // Object.create(proto[, propertiesObject])
    if (values.size() < 2)
      return CJValueP();

    CJValueP protoValue = values[1];

    CJValueP propValue;

    if (values.size() > 2)
      propValue = values[2];

    CJObjTypeFunctionP protoFn = js->valueTypeFunction(protoValue);

    if (! protoFn)
      return CJValueP();

    CJFunction::Values values;

    CJValueP newValue = protoFn->exec(js, values);

    if (protoValue->isObject() && newValue->isObject()) {
      CJObjP newObj = CJValue::cast<CJObj>(newValue);

      newObj->setProtoValue(protoValue);
    }

    if (propValue) {
      if (propValue->type() != CJToken::Type::Object) {
        js->throwTypeError(propValue, "Invalid create properties argument type");
        return CJValueP();
      }

      CJObjectP propObj = CJValue::cast<CJObject>(propValue);

      if (! setObjNamePropertiesValues(js, newValue, propObj))
        return CJValueP();
    }

    return newValue;
  }
  else if (name == "defineProperty") {
    if (values.size() != 4)
      return CJValueP();

    CJValueP obj  = values[1];
    CJValueP prop = values[2];
    CJValueP desc = values[3];

    if (desc->type() == CJToken::Type::Object ||
        desc->type() == CJToken::Type::Dictionary) {
      PropertyValues propValues;

      if (! getPropertyValues(js, desc, propValues))
        return CJValueP();

      if      (obj->hasIndex()) {
        COptLong ind = prop->toInteger();

        if (ind.isValid())
          setIndexPropertyValues(js, obj, ind.getValue(0), propValues);
        else {
          std::string ind = prop->toString();

          setNamePropertyValues(js, obj, ind, propValues);
        }
      }
      else if (obj->hasProperty()) {
        //CJDictionaryP objDict;

        //if (obj->isDictionary())
        //  objDict = CJValue::cast<CJDictionary>(obj);

        std::string ind = prop->toString();

        setNamePropertyValues(js, obj, ind, propValues);
      }
      else {
        js->errorMsg("Invalid object for " + name);
      }
    }
    else {
      js->errorMsg("Invalid descriptor for " + name);
    }

    return CJValueP();
  }
  else if (name == "defineProperties") {
    if (values.size() != 3)
      return CJValueP();

    CJValueP obj = values[1];

    if (obj->type() != CJToken::Type::Object) {
      js->throwTypeError(obj, "Invalid argument type for " + name);
      return CJValueP();
    }

    CJObjectP objObj = CJValue::cast<CJObject>(obj);

    CJValueP prop = values[2];

    if (prop->type() != CJToken::Type::Object) {
      js->throwTypeError(prop, "Invalid argument type for " + name);
      return CJValueP();
    }

    CJObjectP propObj = CJValue::cast<CJObject>(prop);

    if (! setObjNamePropertiesValues(js, obj, propObj))
      return CJValueP();

    return obj;
  }
  else if (name == "freeze") {
    if (values.size() != 2)
      return CJValueP();

    CJValueP obj = values[1];

    if (obj->type() != CJToken::Type::Object) {
      js->throwTypeError(obj, "Invalid argument type for " + name);
      return CJValueP();
    }

    CJObjectP objObj = CJValue::cast<CJObject>(obj);

    objObj->setFrozen(true);

    return objObj;
  }
  else if (name == "getOwnPropertyNames") {
    if (values.size() <= 1) {
      js->errorMsg("Missing value for " + name);
      return CJValueP();
    }

    CJValueP ovalue = values[1];

    if (! ovalue)
      return CJValueP();

    CJArrayP array = js->createArrayValue();

    if      (ovalue->type() == CJToken::Type::Object) {
      CJObjTypeP valueType = CJValue::cast<CJObj>(ovalue)->valueType();

      auto names = valueType->getPropertyNames();

      for (const auto &n : names) {
        array->addValue(js->createStringValue(n));
      }
    }
    else if (ovalue->type() == CJToken::Type::Dictionary) {
      if (values.size() > 1) {
        ovalue = values[1];

        if (ovalue->type() != CJToken::Type::Dictionary) {
          js->errorMsg("Invalid object function type for " + name);
          return CJValueP();
        }
      }

      CJDictionaryP dict = CJValue::cast<CJDictionary>(ovalue);

      for (auto &kv : dict->keyValues()) {
        std::string key = kv.first;

        array->addValue(js->createStringValue(key));
      }
    }
    else if (ovalue->isFunction()) {
      CJFunctionBaseP fn = CJValue::cast<CJFunctionBase>(ovalue);

      CJObjTypeP valueType;

      if (fn->type() == CJFunctionBase::Type::ObjType) {
        CJTypeFunctionP typeFn = CJValue::cast<CJTypeFunction>(ovalue);

        valueType = typeFn->objectType();
      }

      if (valueType) {
        auto names = valueType->getTypePropertyNames();

        for (const auto &n : names) {
          array->addValue(js->createStringValue(n));
        }
      }
      else {
        js->errorMsg("Invalid object function type for" + name);
      }
    }
    else {
      js->errorMsg("Invalid object function type for" + name);
    }

    return array;
  }
  else if (name == "getOwnPropertyDescriptor") {
    if (values.size() <= 2) {
      js->errorMsg("Missing value for " + name);
      return CJValueP();
    }

    CJValueP objVal  = values[1];
    CJValueP nameVal = values[2];

    if (! objVal || ! nameVal)
      return CJValueP();

    if (! objVal->isObject())
      return CJValueP();

    std::string name = nameVal->toString();

    CJObjP obj = CJValue::cast<CJObj>(objVal);

    CJPropertyValue data;

    if (! obj->getPropertyData(js, name, data))
      return CJValueP();

    CJObjectP descObj = js->createObject();

    if (data.value && data.value->type() == CJToken::Type::GetterSetter) {
      CJGetterSetterP gs = CJValue::cast<CJGetterSetter>(data.value);

      descObj->setProperty(js, "get", gs->getter());
      descObj->setProperty(js, "set", gs->setter());
    }
    else {
      descObj->setProperty(js, "value", data.value);

      descObj->setBoolProperty(js, "writable", data.writable.getValue(true));
    }

    descObj->setBoolProperty(js, "enumerable"  , data.enumerable  .getValue(true));
    descObj->setBoolProperty(js, "configurable", data.configurable.getValue(true));

    return descObj;
  }
  else if (name == "getPrototypeOf") {
    if (values.size() <= 1) {
      js->errorMsg("Missing value for " + name);
      return CJValueP();
    }

    CJValueP ovalue = values[1];

    if (! ovalue)
      return CJValueP();

    if (! ovalue->isObject())
      return CJValueP();

    CJObjectP obj = CJValue::cast<CJObject>(ovalue);

    return obj->protoValue();
  }
  else if (name == "isExtensible") {
    if (values.size() != 2)
      return CJValueP();

    CJValueP obj = values[1];

    if (obj->type() != CJToken::Type::Object) {
      js->throwTypeError(obj, "Invalid argument type for " + name);
      return CJValueP();
    }

    CJObjectP objObj = CJValue::cast<CJObject>(obj);

    return js->createBoolValue(objObj->isExtensible());
  }
  else if (name == "isFrozen") {
    if (values.size() != 2)
      return CJValueP();

    CJValueP obj = values[1];

    if (obj->type() != CJToken::Type::Object) {
      js->throwTypeError(obj, "Invalid argument type for " + name);
      return CJValueP();
    }

    CJObjectP objObj = CJValue::cast<CJObject>(obj);

    return js->createBoolValue(objObj->isFrozen());
  }
  else if (name == "isSealed") {
    if (values.size() != 2)
      return CJValueP();

    CJValueP obj = values[1];

    if (obj->type() != CJToken::Type::Object) {
      js->throwTypeError(obj, "Invalid argument type for " + name);
      return CJValueP();
    }

    CJObjectP objObj = CJValue::cast<CJObject>(obj);

    return js->createBoolValue(objObj->isSealed());
  }
  else if (name == "keys") {
    if (values.size() <= 1) {
      js->errorMsg("Missing value for " + name);
      return CJValueP();
    }

    CJValueP ovalue = values[1];

    if (! ovalue)
      return CJValueP();

    CJArrayP array = js->createArrayValue();

    if (ovalue->isObject()) {
      CJObjectP obj = CJValue::cast<CJObject>(ovalue);

      for (const auto &name : obj->propertyNames()) {
        if (! obj->isEnumerableProperty(name))
          continue;

        array->addValue(js->createStringValue(name));
      }
    }

    return array;
  }
  else if (name == "preventExtensions") {
    if (values.size() != 2)
      return CJValueP();

    CJValueP obj = values[1];

    if (obj->type() != CJToken::Type::Object) {
      js->throwTypeError(obj, "Invalid argument type for " + name);
      return CJValueP();
    }

    CJObjectP objObj = CJValue::cast<CJObject>(obj);

    objObj->setExtensible(false);

    return objObj;
  }
  else if (name == "seal") {
    if (values.size() != 2)
      return CJValueP();

    CJValueP obj = values[1];

    if (obj->type() != CJToken::Type::Object) {
      js->throwTypeError(obj, "Invalid argument type for " + name);
      return CJValueP();
    }

    CJObjectP objObj = CJValue::cast<CJObject>(obj);

    objObj->setSealed(true);

    return objObj;
  }
  else if (name == "toString") {
    CJValueP value = values[0];

    // TODO: function [object Function]
    if (value)
      return js->createStringValue("[object " + value->valueType()->name() + "]");
    else
      return js->createStringValue("[object " + this->name() + "]");
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

  CJObjectP obj = CJValue::cast<CJObject>(values[0]);
  assert(obj);

  // object functions
  if      (name == "propertyIsEnumerable") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string ind = values[1]->toString();

    bool b = false;

    if (obj->hasProperty(js, ind, /*inherit*/false))
      b = obj->isEnumerableProperty(ind);

    return js->createBoolValue(b);
  }
  else if (name == "hasOwnProperty") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    return js->createBoolValue(js->hasIndexValue(values[0], values[1], /*inherit*/false));
  }
  else if (name == "isPrototypeOf") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    CJValueP pvalue = values[0];
    CJValueP ovalue = values[1];

    if (! ovalue)
      return js->createBoolValue(false);

    if (! ovalue->isObject())
      return js->createBoolValue(false);

    CJObjectP obj = CJValue::cast<CJObject>(ovalue);

    return js->createBoolValue(obj->isProtoValue(pvalue));
  }
  else {
    js->errorMsg("Invalid object instance function " + name);
  }

  return CJValueP();
}

bool
CJObjectType::
setObjNamePropertiesValues(CJavaScript *js, CJValueP obj, CJObjectP propObj)
{
  for (const auto &name : propObj->propertyNames()) {
    if (! propObj->isEnumerableProperty(name))
      continue;

    CJValueP desc = propObj->getProperty(js, name);

    if (desc->type() != CJToken::Type::Object) {
      js->throwTypeError(desc, "Invalid argument type");
      return false;
    }

    CJObjectP descObj = CJValue::cast<CJObject>(desc);

    //---

    PropertyValues propValues;

    if (! getPropertyValues(js, desc, propValues))
      return false;

    //---

    setNamePropertyValues(js, obj, name, propValues);
  }

  return true;
}

bool
CJObjectType::
getPropertyValues(CJavaScript *js, const CJValueP &desc, PropertyValues &propValues)
{
  CJValueP value, writable, enumerable, configurable, getter, setter;

  if (desc->type() == CJToken::Type::Object) {
    CJObjectP descObj = CJValue::cast<CJObject>(desc);

    propValues.value = descObj->getProperty(js, "value");

    writable     = descObj->getProperty(js, "writable");
    enumerable   = descObj->getProperty(js, "enumerable");
    configurable = descObj->getProperty(js, "configurable");

    getter = descObj->getProperty(js, "get");
    setter = descObj->getProperty(js, "set");
  }
  else {
    CJDictionaryP descDict = CJValue::cast<CJDictionary>(desc);

    propValues.value = descDict->getProperty(js, "value");

    writable     = descDict->getProperty(js, "writable");
    enumerable   = descDict->getProperty(js, "enumerable");
    configurable = descDict->getProperty(js, "configurable");

    getter = descDict->getProperty(js, "get");
    setter = descDict->getProperty(js, "set");
  }

  //---

  if (getter && ! getter->isFunction()) {
    js->throwTypeError(desc, "Invalid get property type");
    return false;
  }

  if (setter && ! setter->isFunction()) {
    js->throwTypeError(desc, "Invalid set property type");
    return false;
  }

  //---

  if (writable    ) propValues.writable     = writable    ->toBoolean();
  if (enumerable  ) propValues.enumerable   = enumerable  ->toBoolean();
  if (configurable) propValues.configurable = configurable->toBoolean();

  if (getter && getter->isFunction() &&
      CJValue::cast<CJFunctionBase>(getter)->type() == CJFunctionBase::Type::User)
    propValues.getter = CJValue::cast<CJFunction>(getter);

  if (setter && setter->isFunction() &&
      CJValue::cast<CJFunctionBase>(setter)->type() == CJFunctionBase::Type::User)
    propValues.setter = CJValue::cast<CJFunction>(setter);

  return true;
}

void
CJObjectType::
setIndexPropertyValues(CJavaScript *js, CJValueP obj, int ind, const PropertyValues &propValues)
{
  assert(obj->hasIndex());

  if     (propValues.getter || propValues.setter) {
    CJGetterSetterP gs = js->createGetterSetter();

    if (propValues.getter)
      gs->setGetter(propValues.getter);

    if (propValues.setter)
      gs->setSetter(propValues.setter);

    gs->setParent(obj);

    obj->setIndexValue(ind, gs);
  }
  else if (propValues.value)
    obj->setIndexValue(ind, propValues.value);

  if (propValues.writable    .isValid())
    obj->setWritableIndex    (ind, propValues.writable    .getValue());
  if (propValues.enumerable  .isValid())
    obj->setEnumerableIndex  (ind, propValues.enumerable  .getValue());
  if (propValues.configurable.isValid())
    obj->setConfigurableIndex(ind, propValues.configurable.getValue());
}

void
CJObjectType::
setNamePropertyValues(CJavaScript *js, CJValueP obj, const std::string &ind,
                      const PropertyValues &propValues)
{
  // . if object not extensible, can edit existing properties but cannot add new properties
  // . if property not configurable you cannot change configurable or enumerable state
  // . If accessor not configurable cannot change getter or setter, can change to value
  // . If data property not configurable cannot change ot to accessor
  // . If data property not configurable cannot change writable from false to true but
  //   can change from true to false
  // . If data property not configurable and not writable you cannot change the value.
  //   You can change the value of a property that is configurable but not writable
  //   (same as configuring to writable set value and set not writable)
  assert(obj->hasProperty());

  CJObjectP     objObj;
  CJDictionaryP objDict;

  if (obj->isObject())
    objObj  = CJValue::cast<CJObject>(obj);
  if (obj->isDictionary())
    objDict = CJValue::cast<CJDictionary>(obj);

  if      (objObj) {
    if (propValues.writable    .isValid())
      objObj->setWritableProperty    (ind, propValues.writable    .getValue());
    if (propValues.enumerable  .isValid())
      objObj->setEnumerableProperty  (ind, propValues.enumerable  .getValue());
    if (propValues.configurable.isValid())
      objObj->setConfigurableProperty(ind, propValues.configurable.getValue());
  }
  else if (objDict) {
    if (propValues.writable    .isValid())
      objDict->setWritableProperty    (ind, propValues.writable    .getValue());
    if (propValues.enumerable  .isValid())
      objDict->setEnumerableProperty  (ind, propValues.enumerable  .getValue());
    if (propValues.configurable.isValid())
      objDict->setConfigurableProperty(ind, propValues.configurable.getValue());
  }

  if      (propValues.getter || propValues.setter) {
    CJGetterSetterP gs = js->createGetterSetter();

    if (propValues.getter)
      gs->setGetter(propValues.getter);

    if (propValues.setter)
      gs->setSetter(propValues.setter);

    gs->setParent(obj);

    if (objObj) {
      objObj->configPropertyValue(ind, gs);
    }
    else {
      obj->configPropertyValue(ind, gs);
    }
  }
  else if (propValues.value) {
    if (objObj) {
      objObj->configPropertyValue(ind, propValues.value);
    }
    else {
      obj->configPropertyValue(ind, propValues.value);
    }
  }
}

//------

CJObject::
CJObject(CJavaScript *js) :
 CJObj(js, CJObjectType::instance(js))
{
}

COptLong
CJObject::
length() const
{
  CJValueP lenValue = propertyValue("length");

  if (! lenValue)
    return COptLong();

  return lenValue->toInteger();
}

void
CJObject::
setLength(long n)
{
  setIntegerProperty(js_, "length", n);
}

CJValueP
CJObject::
getProperty(CJavaScript *js, const std::string &key) const
{
  CJValueP propVal = CJObj::getProperty(js, key);

#if 0
  if (propVal && propVal->type() == CJToken::Type::GetterSetter) {
    CJGetterSetterP gs = CJValue::cast<CJGetterSetter>(propVal);

    CJObjectP obj = CJValue::cast<CJObject>(gs->parent());

    js->pushThis(obj);

    propVal = gs->getValue();

    js->popThis();
  }
#endif

  return propVal;
}

void
CJObject::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
#if 0
  CJValueP propVal = CJObj::getProperty(js, key);

  if (propVal && propVal->type() == CJToken::Type::GetterSetter) {
    CJGetterSetterP gs = CJValue::cast<CJGetterSetter>(propVal);

    CJObjectP obj = CJValue::cast<CJObject>(gs->parent());

    js->pushThis(obj);

    gs->setValue(value);

    js->popThis();

    return;
  }
#endif

  CJObj::setProperty(js, key, value);
}

std::string
CJObject::
toString() const
{
  CJValueP prop = getProperty(js_, "toString");

  if (prop) {
    CJFunctionBaseP fn = js_->valueToFunction(prop);

    if (fn && fn->isUserFunction()) {
      CJFunctionBase::Values values;

      values.push_back(CJValueP());

      CJValueP value = fn->exec(js_, values);

      if (value)
        return value->toString();
    }
  }

  //---

  std::ostringstream ss;

  if (typeName_ != "")
    ss << typeName_ << " ";

  ss << "{";

  int i = 0;

  for (const auto &kv : keyValues_) {
    if (! kv.second.isEnumerable())
      continue;

    if (i > 0)
      ss << ",";

    ss << " " << kv.first << ": ";

    if (kv.second.value)
      ss << *kv.second.value;
    else
      ss << "null";

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

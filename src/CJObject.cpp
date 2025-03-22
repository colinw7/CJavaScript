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
  addTypeFunction(js_, "getOwnPropertySymbols"   , type_);
  addTypeFunction(js_, "getPrototypeOf"          , type_);
  addTypeFunction(js_, "is"                      , type_);
  addTypeFunction(js_, "isExtensible"            , type_);
  addTypeFunction(js_, "isFrozen"                , type_);
  addTypeFunction(js_, "isSealed"                , type_);
  addTypeFunction(js_, "keys"                    , type_);
  addTypeFunction(js_, "preventExtensions"       , type_);
  addTypeFunction(js_, "seal"                    , type_);
  addTypeFunction(js_, "setPrototypeOf"          , type_);
  addTypeFunction(js_, "toString"                , type_);

  addObjectFunction(js_, "hasOwnProperty"      , type_); // TODO: move to base class
  addObjectFunction(js_, "isPrototypeOf"       , type_);
  addObjectFunction(js_, "propertyIsEnumerable", type_); // TODO: move to base class
  addObjectFunction(js_, "toLocaleString"      , type_); // TODO: move to base class
//addObjectFunction(js_, "toSource"            , type_); // TODO: move to base class
  addObjectFunction(js_, "toString"            , type_); // TODO: move to base class
//addObjectFunction(js_, "unwatch"             , type_); // TODO: move to base class
  addObjectFunction(js_, "valueOf"             , type_); // TODO: move to base class
//addObjectFunction(js_, "watch"               , type_); // TODO: move to base class
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

        for (const auto &sname : sourceObj->propertyNames()) {
          if (! sourceObj->isEnumerableProperty(sname))
            continue;

          targetObj->setProperty(js, sname, sourceObj->getProperty(js, sname));
        }
      }
    }

    return targetValue;
  }
  else if (name == "create") {
    // Object.create(proto[, propertiesObject])
    if (values.size() < 2) {
      js->throwTypeError(nullptr, "Invalid arguments for " + name);
      return CJValueP();
    }

    // get prototype object and value
    CJValueP protoValue = values[1];

    if (! protoValue->isObject() && protoValue->type() != CJToken::Type::Null) {
      js->throwTypeError(protoValue, "Invalid argument type for " + name);
      return CJValueP();
    }

    CJValueP propValue;

    if (values.size() > 2)
      propValue = values[2];

    //---

    // create new object using prototype function of object (if any) or basic create
    CJObjTypeFunctionP protoFn = js->valueTypeFunction(protoValue);

    CJObjP newObj;

    if (protoFn) {
      CJFunction::Values values1;

      CJValueP newValue = protoFn->exec(js, values1);

      if (newValue->isObject())
        newObj = CJValue::cast<CJObj>(newValue);
    }
    else {
      newObj = js->createObject();
   }

   // set proto value
   if (newObj)
     newObj->setProtoValue(protoValue);

    //---

    // add properties
    if (propValue) {
      if (propValue->type() != CJToken::Type::Object) {
        js->throwTypeError(propValue, "Invalid create properties argument type");
        return CJValueP();
      }

      CJObjectP propObj = CJValue::cast<CJObject>(propValue);

      if (! setObjNamePropertiesValues(js, newObj, propObj))
        return CJValueP();
    }

    return newObj;
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
        OptLong ind = prop->toInteger();

        if (ind)
          setIndexPropertyValues(js, obj, ind.value_or(0), propValues);
        else {
          std::string ind1 = prop->toString();

          setNamePropertyValues(js, obj, ind1, propValues);
        }
      }
      else if (obj->hasProperty()) {
        //CJDictionaryP objDict;

        //if (obj->isDictionary())
        //  objDict = CJValue::cast<CJDictionary>(obj);

        std::string ind1 = prop->toString();

        setNamePropertyValues(js, obj, ind1, propValues);
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

    //---

    CJValueP ovalue = values[1];

    if (! ovalue)
      return CJValueP();

    if (! ovalue->isObject()) {
      js->throwTypeError(ovalue, "Not an object");
      return CJValueP();
    }

    CJObjP obj = CJValue::cast<CJObj>(ovalue);

    //---

    CJArrayP array = js->createArrayValue();

    //int n = 0;

    auto names = obj->getPropertyNames();

    for (const auto &name1 : names) {
      CJValueP value = js->createStringValue(name1);

      CJPropertyValue propVal(value, "+wec");

      array->addValue(propVal);
    }

    //---

    if      (ovalue->type() == CJToken::Type::Object) {
      CJObjectP obj1 = CJValue::cast<CJObject>(ovalue);


      CJDictionaryP dict = CJValue::cast<CJDictionary>(ovalue);

      for (auto &kv : dict->keyValues()) {
        std::string key = kv.first;

        array->addValue(js->createStringValue(key));
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
    else if (ovalue->isArray()) {
      CJArrayP array1 = CJValue::cast<CJArray>(ovalue);

      //---

      long len = array1->length().value_or(0);

      for (int ind = 0; ind < len; ++ind) {
        CJValueP ivalue1 = js->createStringValue(std::to_string(ind));

        array->addValue(ivalue1);
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
        auto pnames = valueType->getTypePropertyNames();

        for (const auto &n : pnames) {
          array->addValue(js->createStringValue(n));
        }
      }
      else {
        js->errorMsg("Invalid object function type for " + name);
      }
    }
    else {
      js->errorMsg("Invalid object function type for " + name);
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

    std::string name1 = nameVal->toString();

    CJObjP obj = CJValue::cast<CJObj>(objVal);

    CJPropertyValue data;

    if (! obj->getPropertyData(js, name1, data))
      return CJValueP();

    CJObjectP descObj = js->createObject();

    if (data.value() && data.value()->type() == CJToken::Type::GetterSetter) {
      CJGetterSetterP gs = CJValue::cast<CJGetterSetter>(data.value());

      descObj->setProperty(js, "get", gs->getter());
      descObj->setProperty(js, "set", gs->setter());
    }
    else {
      descObj->setProperty(js, "value", data.value());

      descObj->setBoolProperty(js, "writable", data.isWriteable());
    }

    descObj->setBoolProperty(js, "enumerable"  , data.isEnumerable  ());
    descObj->setBoolProperty(js, "configurable", data.isConfigurable());

    return descObj;
  }
  else if (name == "getPrototypeOf") {
    if (values.size() <= 1) {
      js->errorMsg("Missing value for " + name);
      return CJValueP();
    }

    CJValueP ovalue = values[1];

    if (! ovalue || ! ovalue->isObject()) {
      js->throwTypeError(ovalue, "Invalid argument type for " + name);
      return CJValueP();
    }

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

      for (const auto &pname : obj->propertyNames()) {
        if (! obj->isEnumerableProperty(pname))
          continue;

        array->addValue(js->createStringValue(pname));
      }
    }

    if (ovalue->hasIndex()) {
      CJArrayP valueArray;

      if (ovalue->isArray())
        valueArray = CJValue::cast<CJArray>(ovalue);

      long len = ovalue->length().value_or(0);

      for (int ind = 0; ind < len; ++ind) {
        if (valueArray && ! valueArray->isEnumerableIndex(ind))
          continue;

        CJValueP ivalue1 = ovalue->indexValue(ind);

        if (js->isUndefinedValue(ivalue1))
          continue;

        array->addValue(ivalue1);
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
    if (values.size() != 2)
      return js->createBoolValue(false);

    std::string ind = values[1]->toString();

    bool b = false;

    if (obj->hasProperty(js, ind, /*inherit*/false))
      b = obj->isEnumerableProperty(ind);

    return js->createBoolValue(b);
  }
  else if (name == "hasOwnProperty") {
    if (values.size() != 2)
      return js->createBoolValue(false);

    return js->createBoolValue(js->hasIndexValue(values[0], values[1], /*inherit*/false));
  }
  else if (name == "isPrototypeOf") {
    if (values.size() != 2)
      return js->createBoolValue(false);

    CJValueP pvalue = values[0];
    CJValueP ovalue = values[1];

    if (! ovalue || ! ovalue->isObject())
      return js->createBoolValue(false);

    CJObjectP obj1 = CJValue::cast<CJObject>(ovalue);

    return js->createBoolValue(obj1->isProtoValue(pvalue));
  }
  else if (name == "toLocaleString") {
    std::string str = obj->toString();

    return js->createStringValue(str);
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

  if (propValues.writable    )
    obj->setWritableIndex    (ind, propValues.writable    .value());
  if (propValues.enumerable  )
    obj->setEnumerableIndex  (ind, propValues.enumerable  .value());
  if (propValues.configurable)
    obj->setConfigurableIndex(ind, propValues.configurable.value());
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
    if (propValues.writable    )
      objObj->setWritableProperty    (ind, propValues.writable    .value());
    if (propValues.enumerable  )
      objObj->setEnumerableProperty  (ind, propValues.enumerable  .value());
    if (propValues.configurable)
      objObj->setConfigurableProperty(ind, propValues.configurable.value());
  }
  else if (objDict) {
    if (propValues.writable    )
      objDict->setWritableProperty    (ind, propValues.writable    .value());
    if (propValues.enumerable  )
      objDict->setEnumerableProperty  (ind, propValues.enumerable  .value());
    if (propValues.configurable)
      objDict->setConfigurableProperty(ind, propValues.configurable.value());
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

CJObject::
CJObject(const CJObject &obj) :
 CJObj(obj), typeName_(obj.typeName_)
{
}

CJValue *
CJObject::
dup(CJavaScript *) const
{
  return new CJObject(*this);
}

OptLong
CJObject::
length() const
{
  CJValueP lenValue = propertyValue("length");

  if (! lenValue)
    return OptLong();

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

CJNameSpace::Names
CJObject::
getPropertyNames(bool /*pseudo*/) const
{
  return CJNameSpace::Names();
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

  return "[object " + name() + "]";
}

void
CJObject::
print(std::ostream &os) const
{
  CJValueP prop = getProperty(js_, "toString");

  if (prop) {
    CJFunctionBaseP fn = js_->valueToFunction(prop);

    if (fn && fn->isUserFunction()) {
      CJFunctionBase::Values values;

      values.push_back(CJValueP());

      CJObject *th = const_cast<CJObject *>(this);

      CJObjectP dict = CJValue::cast<CJObject>(th->shared_from_this());

      js_->pushThis(dict);

      CJValueP value = fn->exec(js_, values);

      js_->popThis();

      if (value)
        return value->print(os);
    }
  }

  //---

  if (typeName_ != "")
    os << typeName_ << " ";

  os << "{";

  int i = 0;

  for (const auto &kv : keyValues_) {
    if (! kv.second.isEnumerable())
      continue;

    if (i > 0)
      os << ",";

    os << " " << kv.first << ": ";

    if (kv.second.value())
      os << *kv.second.value();
    else
      os << "null";

    ++i;
  }

  os << " }";
}

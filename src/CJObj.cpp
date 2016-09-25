#include <CJObj.h>
#include <CJTypeValue.h>
#include <CJObjTypeFunction.h>
#include <CJObjectTypeFunction.h>
#include <CJavaScript.h>

CJObjType::
CJObjType(CJavaScript *js, const CJToken::Type type, const std::string &name) :
 js_(js), type_(type), name_(name)
{
  addPseudoProperty("length");
  addPseudoProperty("name");
  addPseudoProperty("arguments");
  addPseudoProperty("caller");
}

void
CJObjType::
addTypeFunction(CJavaScript *js, const std::string &name)
{
  CJFunctionBaseP fn(new CJObjTypeFunction(js, name));

  setFunctionProperty(js, fn);

  typeFunctions_[name] = fn;
}

void
CJObjType::
addObjectFunction(CJavaScript *js, const std::string &name)
{
  CJFunctionBaseP fn(new CJObjectTypeFunction(js, name));

  if (! typeFunction())
    setFunctionProperty(js, fn);

  objFunctions_[name] = fn;
}

CJNameSpace::KeyNames
CJObjType::
getTypePropertyNames() const
{
  KeyNames names;

  Names names1 = CJNameSpace::getPropertyNames(/*pseudo*/false);

  for (const auto &n : names1) {
    CJValueP v = getProperty(js_, n);

    if (v->isFunction()) {
      CJFunctionBaseP fn = std::static_pointer_cast<CJFunctionBase>(v);

      if (fn->type() != CJFunctionBase::Type::ObjType)
        continue;
    }

    names.insert(n);
  }

  KeyNames names2 = CJNameSpace::getPseudoPropertyNames();

  for (const auto &n : names2) {
    names.insert(n);
  }

  return names;
}

bool
CJObjType::
isInstanceOf(CJObjTypeP type) const
{
  // is this an inmstance of type
  if (this == type.get())
    return true;

  if (parentType())
    return parentType()->isInstanceOf(type);

  return false;
}

CJValueP
CJObjType::
getProperty(CJavaScript *js, const std::string &key) const
{
  if      (key == "length") {
    return js->createNumberValue(1L);
  }
  else if (key == "name") {
    return js->createStringValue(name_);
  }
  else if (key == "arguments") {
    return js->createNullValue();
  }
  else if (key == "caller") {
    return js->createNullValue();
  }

  return CJNameSpace::getProperty(js, key);
}

//------

CJObj::
CJObj(CJavaScript *js, CJObjTypeP objType) :
 CJDictionary(js, objType), objType_(objType)
{
}

const std::string &
CJObj::
name() const
{
  return objType_->name();
}

void
CJObj::
addVariable(CJavaScript *js, const std::string &name)
{
  setProperty(js, name, CJValueP(new CJTypeValue(js, objType(), name)));
}

bool
CJObj::
hasPropertyValue(const std::string &key) const
{
  return hasProperty(js_, key);
}

CJValueP
CJObj::
propertyValue(const std::string &key) const
{
  return getProperty(js_, key);
}

void
CJObj::
setPropertyValue(const std::string &key, CJValueP value)
{
  setProperty(js_, key, value);
}

bool
CJObj::
hasProperty(CJavaScript *js, const std::string &key) const
{
  // get direct property
  if (CJNameSpace::hasProperty(js, key))
    return true;

  // TODO: prototype
  return false;
}

CJValueP
CJObj::
getProperty(CJavaScript *js, const std::string &key) const
{
  // get direct property
  if (CJNameSpace::hasProperty(js, key))
    return CJNameSpace::getProperty(js, key);

  // get prototype property
  CJValueP protoValue = this->protoValue();

  if (protoValue && protoValue->isObject()) {
    CJObjP obj = std::static_pointer_cast<CJObj>(protoValue);

    if (obj->hasPropertyValue(key))
      return obj->propertyValue(key);
  }

  return objType()->getProperty(js, key);
}

void
CJObj::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJNameSpace::setProperty(js, key, value);
}

bool
CJObj::
isEnumerableProperty(const std::string &key) const
{
  // check direct property
  if (CJNameSpace::hasProperty(js_, key))
    return CJNameSpace::isEnumerableProperty(key);

  // check prototype property
  CJValueP protoValue = this->protoValue();

  if (protoValue && protoValue->isObject()) {
    CJObjP obj = std::static_pointer_cast<CJObj>(protoValue);

    if (obj->CJNameSpace::hasProperty(js_, key))
      return obj->isEnumerableProperty(key);
  }

  if (objType()->CJNameSpace::hasProperty(js_, key))
    return objType()->CJNameSpace::isEnumerableProperty(key);

  return false;
}

CJValue::KeyNames
CJObj::
propertyNames() const
{
  std::set<std::string> nameSet;

  CJValue::KeyNames names1 = CJDictionary::propertyNames();

  CJValueP protoValue = this->protoValue();

  if (! protoValue)
    return names1;

  CJValue::KeyNames names2 = protoValue->propertyNames();

  if (names2.empty())
    return names1;

  //---

  CJValue::KeyNames names;

  for (const auto &n : names1)
    nameSet.insert(n);

  for (const auto &n : names2)
    nameSet.insert(n);

  for (const auto &n : nameSet)
    names.push_back(n);

  return names;
}

CJValueP
CJObj::
protoValue() const
{
  CJObjTypeFunctionP typeFn = objType()->typeFunction();

  if (! typeFn)
    return CJValueP();

  CJValueP protoValue = typeFn->getProperty(js_, "prototype");

  if (! protoValue || protoValue.get() == this)
    return CJValueP();

  return protoValue;
}

CJValueP
CJObj::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  assert(false);
}

std::string
CJObj::
toString() const
{
  return "[object " + name() + "]";
}

void
CJObj::
print(std::ostream &os) const
{
  os << toString();
}

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

CJValueP
CJObj::
getProperty(CJavaScript *js, const std::string &name) const
{
  // get direct property
  if (CJNameSpace::hasProperty(js, name))
    return CJNameSpace::getProperty(js, name);

  // get constructor property
  CJObjTypeFunctionP typeFn = objType()->typeFunction();

  CJValueP value;

  if (typeFn)
    value = typeFn->getProperty(js, "prototype");

  if (value && value->isObject()) {
    CJObjP obj = std::static_pointer_cast<CJObj>(value);

    if (obj->hasPropertyValue(name))
      return obj->propertyValue(name);
  }

  return objType()->getProperty(js, name);
}

void
CJObj::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJNameSpace::setProperty(js, name, value);
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

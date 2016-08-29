#include <CJObj.h>
#include <CJTypeValue.h>
#include <CJTypeFunction.h>
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
  setFunctionProperty(js, CJFunctionP(new CJTypeFunction(js, name)));
}

void
CJObjType::
addObjectFunction(CJavaScript *js, const std::string &name)
{
  setFunctionProperty(js, CJFunctionP(new CJObjectTypeFunction(js, name)));
}

CJNameSpace::KeyNames
CJObjType::
getTypePropertyNames() const
{
  KeyNames names;

  Names names1 = CJNameSpace::getPropertyNames(/*pseudo*/false);

  for (const auto &n : names1) {
    CJValueP v = getProperty(js_, n);

    if (v->type() == CJToken::Type::Function) {
      CJFunctionP fn = std::static_pointer_cast<CJFunction>(v);

      if (fn->type() != CJFunction::Type::Type)
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
CJObj(CJavaScript *js, CJObjTypeP type) :
 CJDictionary(js, type), type_(type)
{
}

const std::string &
CJObj::
name() const
{
  return type_->name();
}

void
CJObj::
addVariable(CJavaScript *js, const std::string &name)
{
  setProperty(js, name, CJValueP(new CJTypeValue(js, type(), name)));
}

CJValueP
CJObj::
getProperty(CJavaScript *js, const std::string &name) const
{
  if (CJNameSpace::hasProperty(js, name))
    return CJNameSpace::getProperty(js, name);

  if (CJNameSpace::hasProperty(js, "prototype")) {
    CJValueP value = getProperty(js, "prototype");

    if (value && value->type() == CJToken::Type::Dictionary) {
      CJDictionaryP dict = std::static_pointer_cast<CJDictionary>(value);

      if (dict->hasProperty(js, name))
        return dict->getProperty(js, name);
    }
  }

  return type()->getProperty(js, name);
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

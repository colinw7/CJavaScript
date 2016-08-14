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
  addPseudoProperty("prototype");
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
    CJValueP v = getProperty(n);

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
getProperty(const std::string &key) const
{
  if      (key == "length") {
    return js_->createNumberValue(1L);
  }
  else if (key == "name") {
    return js_->createStringValue(name_);
  }
  else if (key == "arguments") {
    return js_->createNullValue();
  }
  else if (key == "caller") {
    return js_->createNullValue();
  }
  else if (key == "prototype") {
    return js_->createNullValue();
  }

  return CJNameSpace::getProperty(key);
}

//------

CJObj::
CJObj(CJObjTypeP type) :
 CJValue(type), type_(type)
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
  setProperty(name, CJValueP(new CJTypeValue(js, type(), name)));
}

CJValueP
CJObj::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  assert(false);
}

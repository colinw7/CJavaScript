#include <CJFunctionBase.h>
#include <CJCallFunction.h>
#include <CJObjValue.h>
#include <CJavaScript.h>

CJFunctionBaseType::NameTypeMap CJFunctionBaseType::nameTypeMap_;

CJObjTypeP
CJFunctionBaseType::
instance(CJavaScript *js, const std::string &name)
{
  auto p = nameTypeMap_.find(name);

  if (p == nameTypeMap_.end()) {
    CJObjTypeP type(new CJFunctionBaseType(js, name));

    p = nameTypeMap_.insert(p, NameTypeMap::value_type(name, type));

    //js->addObjectType(name, type);

    //type->addTypeFunction(js, "isGenerator");
    //type->addTypeFunction(js, "toSource");
    type->addTypeFunction(js, "toString");
  }

  return (*p).second;
}

CJFunctionBaseType::
CJFunctionBaseType(CJavaScript *js, const std::string &name) :
 CJObjType(js, CJToken::Type::FunctionBase, name), name_(name)
{
}

CJValueP
CJFunctionBaseType::
execType(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  // values[0] is CJFunctionFunction

  //---

  if (name == "toString") {
    return js->createStringValue("function Function() { }");
  }
  else {
    js->errorMsg("Invalid function type function " + name);
  }

  return CJValueP();
}

CJValueP
CJFunctionBaseType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJFunctionBase::
CJFunctionBase(CJavaScript *js, const std::string &name, Type type) :
 CJObj(js, CJFunctionBaseType::instance(js, name)), name_(name), type_(type)
{
  prototype_ = js->createObject();

  setProperty(js, "prototype", prototype_);

  //addPseudoProperty("length");
}

CJFunctionBase::
CJFunctionBase(CJavaScript *js, const CJObjTypeP &objType, const std::string &name, Type type) :
 CJObj(js, objType), name_(name), type_(type)
{
  prototype_ = js->createObject();

  setProperty(js, "prototype", prototype_);

  //addPseudoProperty("length");
}

CJFunctionBase::
CJFunctionBase(const CJFunctionBase &f) :
 CJObj(f), name_(f.name_), type_(f.type_), prototype_(f.prototype_), numArgs_(f.numArgs_)
{
  //addPseudoProperty("length");
}

void
CJFunctionBase::
addFunctionMethods(CJavaScript *js, CJFunctionBaseP fn, CJObjTypeP type)
{
  fn->setProperty(js, "bind" ,
    CJValueP(new CJCallFunction(js, fn, CJCallFunction::Type::Bind , type)));
  fn->setProperty(js, "call" ,
    CJValueP(new CJCallFunction(js, fn, CJCallFunction::Type::Call , type)));
  fn->setProperty(js, "apply",
    CJValueP(new CJCallFunction(js, fn, CJCallFunction::Type::Apply, type)));

  CJValueP lenVal(new CJObjValue(js, fn, "length"));

  fn->setPropertyData(js, "length", CJPropertyValue(lenVal, "-wec"));
}

CJValueP
CJFunctionBase::
getProperty(CJavaScript *js, const std::string &key) const
{
  if (key == "length")
    return js->createNumberValue(numArgs());

  return CJObj::getProperty(js, key);
}

void
CJFunctionBase::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJObj::setProperty(js, key, value);
}

std::string
CJFunctionBase::
toString() const
{
  return name_ + "()";
}

void
CJFunctionBase::
print(std::ostream &os) const
{
  os << name_ << "()";
}

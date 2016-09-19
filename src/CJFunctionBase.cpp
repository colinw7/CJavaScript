#include <CJFunctionBase.h>
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
  prototype_ = CJDictionaryP(new CJDictionary(js));

  setProperty(js, "prototype", prototype_);
}

CJFunctionBase::
CJFunctionBase(CJavaScript *js, const CJObjTypeP &objType, const std::string &name, Type type) :
 CJObj(js, objType), name_(name), type_(type)
{
}

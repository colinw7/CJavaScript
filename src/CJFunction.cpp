#include <CJFunction.h>
#include <CJavaScript.h>

CJFunctionType::NameTypeMap CJFunctionType::nameTypeMap_;

CJObjTypeP
CJFunctionType::
instance(CJavaScript *js, const std::string &name)
{
  auto p = nameTypeMap_.find(name);

  if (p == nameTypeMap_.end())
    p = nameTypeMap_.insert(p, NameTypeMap::value_type(name, CJObjTypeP()));

  (*p).second = CJObjTypeP(new CJFunctionType(js, name));

  js->addObjectType(name, (*p).second);

  return (*p).second;
}

CJFunctionType::
CJFunctionType(CJavaScript *js, const std::string &name) :
 CJObjType(js, CJToken::Type::Function, name), name_(name)
{
}

CJValueP
CJFunctionType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJFunction::
CJFunction(CJavaScript *js, const std::string &name, Type type) :
 CJObj(js, CJFunctionType::instance(js, name)), name_(name), type_(type)
{
  setProperty(js, "prototype", CJDictionaryP(new CJDictionary(js)));
}

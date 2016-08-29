#include <CJDictionary.h>
#include <CJavaScript.h>

CJObjTypeP CJDictionaryType::type_;

CJObjTypeP
CJDictionaryType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJDictionaryType(js));

    js->addObjectType("dictionary", type_);
  }

  return type_;
}

CJDictionaryType::
CJDictionaryType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Dictionary, "dictionary")
{
}

CJValueP
CJDictionaryType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJDictionary::
CJDictionary(CJavaScript *js, CJObjTypeP type) :
 CJValue(type), CJNameSpace(), js_(js)
{
}

CJDictionary::
CJDictionary(CJavaScript *js, const std::string &name, const KeyValues &keyValues) :
 CJValue(CJDictionaryType::instance(js)), CJNameSpace(keyValues), js_(js), name_(name)
{
}

std::vector<std::string>
CJDictionary::
getFunctionNames() const
{
  std::vector<std::string> names;

  for (const auto &f : keyValues_) {
    if (f.second->type() == CJToken::Type::Function)
      names.push_back(f.first);
  }

  return names;
}

std::vector<std::string>
CJDictionary::
getVariableNames() const
{
  std::vector<std::string> names;

  for (const auto &v : keyValues_) {
    if (v.second->type() == CJToken::Type::Var)
      names.push_back(v.first);
  }

  return names;
}

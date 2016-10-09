#include <CJExecDictionary.h>
#include <CJObject.h>
#include <CJString.h>
#include <CJGetterSetter.h>
#include <CJExecExpression.h>
#include <CJavaScript.h>

CJExecDictionary::
CJExecDictionary() :
 CJToken(CJToken::Type::Dictionary)
{
}

void
CJExecDictionary::
addDictionaryValue(const CJValueP &key, CJExecExpressionP expr)
{
  values_.push_back(DictionaryValue(key, expr));
}

CJGetterSetterP
CJExecDictionary::
getGetterSetter(const std::string &name)
{
  auto p = gsMap_.find(name);

  if (p != gsMap_.end())
    return (*p).second;

  return CJGetterSetterP();
}

void
CJExecDictionary::
setGetterSetter(const std::string &name, CJGetterSetterP gs)
{
  gsMap_[name] = gs;
}

CJValueP
CJExecDictionary::
exec(CJavaScript *js)
{
  CJObjectP dict = js->createObject();

  for (const auto &v : values_) {
    CJValueP value = v.expr->exec(js);

    if (! v.key)
      continue;

    dict->setProperty(js, v.key->toString(), value);
  }

  for (const auto &gs : gsMap_) {
    gs.second->setParent(dict);

    dict->setProperty(js, gs.first, gs.second);
  }

  return dict;
}

std::string
CJExecDictionary::
toString() const
{
  std::string str;

  str += "{";

  int i = 0;

  for (const auto &v : values_) {
    if (i > 0) str += " ";

    str += v.key->toString() + ":" + v.expr->toString();

    ++i;
  }

  str += "}";

  return str;
}

void
CJExecDictionary::
print(std::ostream &os) const
{
  os << "{";

  int i = 0;

  for (const auto &v : values_) {
    if (i > 0) os << " ";

    os << *v.key << ":" << *v.expr;

    ++i;
  }

  os << "}";
}

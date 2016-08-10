#include <CJExecDictionary.h>
#include <CJDictionary.h>
#include <CJExecExpression.h>

CJExecDictionary::
CJExecDictionary() :
 CJToken(CJToken::Type::Dictionary)
{
}

CJValueP
CJExecDictionary::
exec(CJavaScript *js)
{
  CJDictionary *dict = new CJDictionary(js);

  for (const auto &v : values_) {
    CJValueP value = v.expr->exec(js);

    dict->setProperty(v.key->toString(), value);
  }

  return CJValueP(dict);
}

void
CJExecDictionary::
print(std::ostream &os) const
{
  os << "{";

  int i = 0;

  for (const auto &v : values_) {
    if (i > 0) os << " ";

    os << v.key->toString() << ":" << *v.expr;

    ++i;
  }

  os << "}";
}

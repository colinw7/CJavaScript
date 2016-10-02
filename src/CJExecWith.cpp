#include <CJExecWith.h>
#include <CJExecIdentifiers.h>
#include <CJavaScript.h>

CJExecWith::
CJExecWith() :
 CJToken(CJToken::Type::With)
{
}

CJValueP
CJExecWith::
exec(CJavaScript *js)
{
  CJValueP varValue;

  if (identifiers_)
    varValue = js->lookupValue(identifiers_->identifiers());

  CJValueP value;

  if      (varValue && varValue->type() == CJToken::Type::Object) {
    CJObjectP obj = CJValue::cast<CJObject>(varValue);

    js->startScope(obj);

    if (block_)
      value = block_->exec(js);

    js->endScope();
  }
  else if (varValue && varValue->type() == CJToken::Type::Dictionary) {
    CJDictionaryP dict = CJValue::cast<CJDictionary>(varValue);

    js->startScope(dict);

    if (block_)
      value = block_->exec(js);

    js->endScope();
  }
  else {
    if (block_)
      value = block_->exec(js);
  }

  return value;
}

std::string
CJExecWith::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJExecWith::
print(std::ostream &os) const
{
  os << "with (";

  if (identifiers_)
    os << *identifiers_;

  os << ") {";

  if (block_)
    os << *block_;

  os << "}";
}

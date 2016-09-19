#include <CJTokenValue.h>
#include <CJavaScript.h>

CJObjTypeP CJTokenValueType::type_;

CJObjTypeP
CJTokenValueType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJTokenValueType(js));

    js->addObjectType("tokenValue", type_);
  }

  return type_;
}

CJTokenValueType::
CJTokenValueType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::TokenValue, "tokenValue")
{
}

CJValueP
CJTokenValueType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJTokenValue::
CJTokenValue(CJavaScript *js, const Tokens &tokens, const CJValueP &value) :
 CJValue(CJTokenValueType::instance(js)), tokens_(tokens), value_(value)
{
}

std::string
CJTokenValue::
toString() const
{
  if (value_)
    return value_->toString();
  else
    return "";
}

double
CJTokenValue::
toReal() const
{
  if (value_)
    return value_->toReal();
  else
    return 0.0;
}

bool
CJTokenValue::
toBoolean() const
{
  if (value_)
    return value_->toBoolean();
  else
    return false;
}

void
CJTokenValue::
addToken(CJTokenP token)
{
  tokens_.push_back(token);
}

void
CJTokenValue::
setValue(CJValueP value)
{
  value_ = value;
}

void
CJTokenValue::
print(std::ostream &os) const
{
  int i = 0;

  for (const auto &t : tokens_) {
    if (i > 0) os << " ";

    os << *t;

    ++i;
  }
}

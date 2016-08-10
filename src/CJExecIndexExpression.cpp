#include <CJExecIndexExpression.h>
#include <CJavaScript.h>

CJExecIndexExpression::
CJExecIndexExpression() :
 CJExecExpression(CJToken::Type::IndexExpression)
{
}

CJValueP
CJExecIndexExpression::
exec(CJavaScript *js)
{
  CJValueP value;

  //---

  // get indexed value
  CJValueP avalue;

  if (identifiers_ || expr_) {
    if (identifiers_) {
      CJLValueP varValue = js->lookupProperty(identifiers_->identifiers());

      if (varValue)
        avalue = varValue->value();
    }
    else {
      avalue = expr_->exec(js);
    }
  }
  else
    avalue = evalue_;

  if (! avalue) {
    js->errorMsg("Invalid index value");
    return value;
  }

  //---

  if (! iexpr_)
    return value;

  // index value
  CJValueP res;

  if      (avalue->type() == CJToken::Type::Array) {
    CJArray *array = avalue->cast<CJArray>();

    CJValueP ivalue = iexpr_->exec(js);

    long ind = ivalue->toInteger();

    res = array->indexValue(ind);
  }
  else if (avalue->type() == CJToken::Type::Dictionary) {
    CJDictionary *dict = avalue->cast<CJDictionary>();

    CJValueP ivalue = iexpr_->exec(js);

    res = dict->getProperty(ivalue->toString());
  }
  else if (avalue->type() == CJToken::Type::String) {
    CJString *str = avalue->cast<CJString>();

    CJValueP ivalue = iexpr_->exec(js);

    long ind = ivalue->toInteger();

    res = str->indexValue(ind);
  }
  else {
    CJValueP ivalue = iexpr_->exec(js);

    long ind = ivalue->toInteger();

    if (! avalue->hasIndex()) {
      js->errorMsg("Value not an array for index");
      return res;
    }

    res = avalue->indexValue(ind);
  }

  if (! res)
    res = CJValueP(new CJUndefined(js));

  return res;
}

void
CJExecIndexExpression::
print(std::ostream &os) const
{
  if      (expr_) {
    if (iexpr_)
      os << *expr_ << "[" << *iexpr_ << "]";
    else
      os << *expr_ << "[]";
  }
  else if (identifiers_) {
    if (iexpr_)
      os << *identifiers_ << "[" << *iexpr_ << "]";
    else
      os << *identifiers_ << "[]";
  }
}

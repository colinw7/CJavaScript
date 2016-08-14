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
    return CJValueP();
  }

  // index value
  return indexValue(js, avalue);
}

CJValueP
CJExecIndexExpression::
indexValue(CJavaScript *js, CJValueP avalue)
{
  if (! iexpr_)
    return CJValueP();

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
    res = js->createUndefinedValue();

  return res;
}

void
CJExecIndexExpression::
setIndexValue(CJavaScript *js, CJValueP avalue, CJValueP rvalue)
{
  if (! iexpr_)
    return;

  // set index value
  if      (avalue->type() == CJToken::Type::Array) {
    CJArray *array = avalue->cast<CJArray>();

    CJValueP ivalue = iexpr_->exec(js);

    long ind = ivalue->toInteger();

    array->setIndexValue(ind, rvalue);
  }
  else if (avalue->type() == CJToken::Type::Dictionary) {
    CJDictionary *dict = avalue->cast<CJDictionary>();

    CJValueP ivalue = iexpr_->exec(js);

    dict->setProperty(ivalue->toString(), rvalue);
  }
  else if (avalue->type() == CJToken::Type::String) {
    CJString *str = avalue->cast<CJString>();

    CJValueP ivalue = iexpr_->exec(js);

    long ind = ivalue->toInteger();

    str->setIndexValue(ind, rvalue);
  }
  else {
    CJValueP ivalue = iexpr_->exec(js);

    long ind = ivalue->toInteger();

    if (! avalue->hasIndex()) {
      js->errorMsg("Value not an array for index");
      return;
    }

    avalue->setIndexValue(ind, rvalue);
  }
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

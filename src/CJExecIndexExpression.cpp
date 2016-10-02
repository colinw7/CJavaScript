#include <CJExecIndexExpression.h>
#include <CJExecIdentifiers.h>
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
    js->throwReferenceError(this, "Invalid index value");
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
  CJValueP ivalue = indValue(js);

  if (! ivalue)
    return CJValueP();

  CJValueP res;

  if (! js->indexValue(avalue, ivalue, res)) {
    js->throwTypeError(this, "Value not an array or property for index");
    return CJValueP();
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
  CJValueP ivalue = indValue(js);

  if (! js->setIndexValue(avalue, ivalue, rvalue)) {
    js->throwSyntaxError(this, "Value not an array or property for index");
    return;
  }
}

bool
CJExecIndexExpression::
decodeExpressions(CJExecIdentifiersP &identifiers, std::vector<CJExecExpressionP> &indices) const
{
  if (! iexpr_)
    return false;

  indices.push_back(iexpr_);

  if (identifiers_) {
    identifiers = identifiers_;

    return true;
  }

  CJExecIndexExpressionP iexpr = lindexExpression();

  if (iexpr) {
    CJExecIdentifiersP             identifiers1;
    std::vector<CJExecExpressionP> indices1;

    if (! iexpr->decodeExpressions(identifiers1, indices1))
      return false;

    if (identifiers1 && ! identifiers)
      identifiers = identifiers1;

    for (const auto &i : indices)
      indices1.push_back(i);

    indices = indices1;

    return true;
  }

  return false;
}

CJExecIndexExpressionP
CJExecIndexExpression::
lindexExpression() const
{
  if (! expr_ || expr_->type() != CJToken::Type::IndexExpression)
    return CJExecIndexExpressionP();

  return std::static_pointer_cast<CJExecIndexExpression>(expr_);
}

CJValueP
CJExecIndexExpression::
indValue(CJavaScript *js)
{
  return iexpr_->exec(js);
}

std::string
CJExecIndexExpression::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJExecIndexExpression::
print(std::ostream &os) const
{
  if      (expr_) {
    if (iexpr_)
      os << *expr_ << "[" << *iexpr_ << "]";
    else
      os << "[" << *expr_ << "]";
  }
  else if (identifiers_) {
    if (iexpr_)
      os << *identifiers_ << "[" << *iexpr_ << "]";
    else
      os << "[" << *identifiers_ << "]";
  }
  else if (evalue_)
    os << *evalue_;
}

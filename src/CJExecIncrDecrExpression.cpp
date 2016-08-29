#include <CJExecIncrDecrExpression.h>
#include <CJExecIdentifiers.h>
#include <CJavaScript.h>

CJExecIncrDecrExpression::
CJExecIncrDecrExpression() :
 CJExecExpression(CJToken::Type::IncrDecrExpression)
{
}

CJValueP
CJExecIncrDecrExpression::
exec(CJavaScript *js)
{
  CJValueP value;

  if (! identifiers_ || ! op_)
    return value;

  CJLValueP varValue = js->lookupProperty(identifiers_->identifiers(), /*create*/true);

  CJOperator::Type opType = op_->type();

  if (varValue)
    value = varValue->value();

  CJValueP value1 = js->execUnaryOp(opType, value);

  varValue->setValue(value1);

  return (post_ ? value : value1);
}

void
CJExecIncrDecrExpression::
print(std::ostream &os) const
{
  if (! op_) return;

  if (identifiers_) {
    if (post_)
      os << *identifiers_ << *op_;
    else
      os << *op_ << *identifiers_;
  }
  else {
    if (post_)
      os << *expr_ << *op_;
    else
      os << *op_ << *expr_;
  }

  os << ";";
}

#include <CJExecQuestion.h>
#include <CJExecExpression.h>
#include <CJavaScript.h>

CJExecQuestion::
CJExecQuestion(CJExecExpressionP bexpr) :
 CJToken(CJToken::Type::Question), bexpr_(std::move(bexpr))
{
}

CJValueP
CJExecQuestion::
exec(CJavaScript *js)
{
  if (! bexpr_ || ! expr1_ || ! expr2_)
    return CJValueP();

  CJValueP value = bexpr_->exec(js);

  CJValueP value1;

  if (value) {
    if (value->toBoolean())
      value1 = expr1_->exec(js);
    else
      value1 = expr2_->exec(js);
  }

  return value1;
}

void
CJExecQuestion::
print(std::ostream &os) const
{
  if (bexpr_ && expr1_ && expr2_) {
    os << *bexpr_ << " ? " << *expr1_ << " : " << *expr2_;
  }
}

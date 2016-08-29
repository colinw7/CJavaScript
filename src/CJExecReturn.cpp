#include <CJExecReturn.h>
#include <CJExecExpression.h>
#include <CJavaScript.h>

CJExecReturn::
CJExecReturn() :
 CJToken(CJToken::Type::ExecReturn)
{
}

CJValueP
CJExecReturn::
exec(CJavaScript *js)
{
  CJValueP value;

  if (expr_)
    value = expr_->exec(js);

  CJExecBlockP block = js->getCurrentBlock();

  if (block) {
    block->setRetVal(value);

    block->setReturnFlag(true);
  }

  return value;
}

void
CJExecReturn::
print(std::ostream &os) const
{
  if (expr_)
    os << "return " << *expr_;
}

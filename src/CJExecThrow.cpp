#include <CJExecThrow.h>
#include <CJExecExpression.h>
#include <CJavaScript.h>
#include <CJError.h>

CJExecThrow::
CJExecThrow() :
 CJToken(CJToken::Type::Throw)
{
}

CJValueP
CJExecThrow::
exec(CJavaScript *js)
{
  CJValueP value;

  if (expr_)
    value = expr_->exec(js);

  std::string msg;

  if (value)
    msg = value->toString();

  CJError *error = new CJError(js);

  error->setMessage(msg);

  js->throwError(this, CJErrorBaseP(error));

  return CJValueP();
}

void
CJExecThrow::
print(std::ostream &os) const
{
  if (expr_) {
    os << "throw " << *expr_;
  }
}

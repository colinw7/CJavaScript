#include <CJExecThrow.h>
#include <CJExecExpression.h>
#include <CJavaScript.h>

CJExecThrow::
CJExecThrow() :
 CJToken(CJToken::Type::Throw)
{
}

CJValueP
CJExecThrow::
exec(CJavaScript *js)
{
  if (expr_) {
    CJValueP value = expr_->exec(js);

    if (value)
      std::cerr << value->toString() << std::endl;
    else
      std::cerr << "null" << std::endl;
  }

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

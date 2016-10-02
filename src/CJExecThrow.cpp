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
  // evaluate expression
  CJValueP value;

  if (expr_)
    value = expr_->exec(js);

  // create error object (if not one already)
  CJErrorBaseP perror;

  if (value && value->isError()) {
    perror = CJValue::cast<CJErrorBase>(value);
  }
  else {
    CJError *error = new CJError(js);

    error->setValue(value);

    perror = CJErrorBaseP(error);
  }

  // throw error
  js->throwError(this, perror);

  return CJValueP();
}

std::string
CJExecThrow::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJExecThrow::
print(std::ostream &os) const
{
  if (expr_) {
    os << "throw " << *expr_;
  }
}

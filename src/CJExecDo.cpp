#include <CJExecDo.h>
#include <CJExecExpressionList.h>
#include <CJavaScript.h>

CJExecDo::
CJExecDo() :
 CJToken(CJToken::Type::ExecDo)
{
}

CJValueP
CJExecDo::
exec(CJavaScript *js)
{
  if (! exprList_ || ! block_)
    return CJValueP();

  CJValueP value, value1;

  do {
    js->startBlock(block_);

    value1 = block_->exec(js);

    js->endBlock();

    if (block_->isBreakFlag())
      break;

    value = exprList_->exec(js);
  }
  while (value && value->toBoolean());

  return value1;
}

std::string
CJExecDo::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJExecDo::
print(std::ostream &os) const
{
  if (exprList_ && block_) {
    os << "do " << *block_ << "while (" << *exprList_ << ")";
  }
}

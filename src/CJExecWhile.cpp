#include <CJExecWhile.h>
#include <CJExecExpressionList.h>
#include <CJavaScript.h>

CJExecWhile::
CJExecWhile() :
 CJToken(CJToken::Type::ExecWhile)
{
}

CJValueP
CJExecWhile::
exec(CJavaScript *js)
{
  if (! exprList_ || ! block_)
    return CJValueP();

  CJValueP value = exprList_->exec(js);

  CJValueP value1;

  while (value && value->toBoolean()) {
    js->startBlock(block_);

    value1 = block_->exec(js);

    js->endBlock();

    if (block_->isBreakFlag())
      break;

    value = exprList_->exec(js);
  }

  return value1;
}

std::string
CJExecWhile::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJExecWhile::
print(std::ostream &os) const
{
  if (exprList_ && block_) {
    os << "while (" << *exprList_ << ") " << *block_;
  }
}

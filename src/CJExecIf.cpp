#include <CJExecIf.h>
#include <CJExecExpressionList.h>
#include <CJavaScript.h>

CJExecIf::
CJExecIf() :
 CJToken(CJToken::Type::If)
{
}

CJValueP
CJExecIf::
exec(CJavaScript *js)
{
  if (! ifBlock_.exprList || ! ifBlock_.block)
    return CJValueP();

  // run if block if expression is true
  CJValueP value = ifBlock_.exprList->exec(js);

  if (value && value->toBoolean()) {
    js->startBlock(ifBlock_.block);

    CJValueP value1 = ifBlock_.block->exec(js);

    js->endBlock();

    return CJValueP();
  }

  // run else if block if expression is true
  for (const auto &b : elseIfBlocks_) {
    CJValueP value = b.exprList->exec(js);

    if (value && value->toBoolean()) {
      js->startBlock(b.block);

      CJValueP value1 = b.block->exec(js);

      js->endBlock();

      return CJValueP();
    }
  }

  // nothing true so run else if any
  if (elseBlock_.block) {
    js->startBlock(elseBlock_.block);

    CJValueP value1 = elseBlock_.block->exec(js);

    js->endBlock();

    return CJValueP();
  }

  return CJValueP();
}

std::string
CJExecIf::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJExecIf::
print(std::ostream &os) const
{
  if (ifBlock_.exprList && ifBlock_.block) {
    os << "if (" << *ifBlock_.exprList << ") " << *ifBlock_.block;
  }

  for (const auto &b : elseIfBlocks_) {
    os << " else if (" << *b.exprList << ") " << *b.block;
  }

  if (elseBlock_.block) {
    os << " else " << *elseBlock_.block;
  }
}

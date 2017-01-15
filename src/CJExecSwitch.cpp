#include <CJExecSwitch.h>
#include <CJExecExpressionList.h>
#include <CJExecExpression.h>
#include <CJavaScript.h>

CJExecSwitch::
CJExecSwitch() :
 CJToken(CJToken::Type::Switch)
{
}

CJValueP
CJExecSwitch::
exec(CJavaScript *js)
{
  if (! exprList_)
    return CJValueP();

  CJValueP value = exprList_->exec(js);

  if (! value)
    return CJValueP();

  //---

  // find index of first block which matches switch expression
  int match = -1;

  for (uint i = 0; i < caseBlocks_.size(); ++i) {
    const SwitchBlock &block = caseBlocks_[i];

    if (! block.expr)
      continue;

    CJValueP v = block.expr->exec(js);

    if (js->cmp(v, value) == 0) {
      match = i;
      break;
    }
  }

  //---

  // if no match then just exec default
  if (match == -1) {
    const SwitchBlock &block = defaultBlock_;

    (void) execBlock(js, block.block);

    return CJValueP();
  }

  //---

  // if match then run each block until we hit a break
  for (uint i = match; i < caseBlocks_.size(); ++i) {
    const SwitchBlock &block = caseBlocks_[i];

    bool done = execBlock(js, block.block);

    if (done)
      break;
  }

  return CJValueP();
}

bool
CJExecSwitch::
execBlock(CJavaScript *js, CJExecBlockP block)
{
  if (! block) return false;

  js->startBlock(block);

  block->exec(js);

  bool done = block->isBreakFlag();

  js->endBlock();

  return done;
}

std::string
CJExecSwitch::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJExecSwitch::
print(std::ostream &os) const
{
  os << "switch (";

  if (exprList_)
    os << *exprList_;

  os << ") {" << std::endl;

  for (const auto &c : caseBlocks_) {
    if (c.expr && c.block)
      os << "case " << *c.expr << " : " << *c.block << std::endl;
  }

  if (defaultBlock_.block) {
    os << "default : " << *defaultBlock_.block << std::endl;
  }

  os << "}";
}


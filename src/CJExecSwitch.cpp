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

  CJValueP value1;

  bool found = false;

  for (const auto &c : caseBlocks_) {
    if (c.expr) {
      CJValueP v = c.expr->exec(js);

      if (js->cmp(v, value) == 0) {
        if (c.block) {
          js->startBlock(c.block);

          c.block->exec(js);

          js->endBlock();
        }

        found = true;

        break;
      }
    }
  }

  if (! found) {
    if (defaultBlock_.block) {
      js->startBlock(defaultBlock_.block);

      defaultBlock_.block->exec(js);

      js->endBlock();
    }
  }

  return CJValueP();
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


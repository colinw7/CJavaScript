#include <CJExecFor.h>
#include <CJExecIdentifiers.h>
#include <CJExecExpression.h>
#include <CJExecExpressionList.h>
#include <CJExecVar.h>
#include <CJavaScript.h>

CJExecFor::
CJExecFor() :
 CJToken(CJToken::Type::For)
{
}

CJValueP
CJExecFor::
exec(CJavaScript *js)
{
  // for in
  if      (inExpr_) {
    if (! identifiers_) {
      js->errorMsg("Missing variable name for for in");
      return CJValueP();
    }

    CJLValueP ivalue = js->lookupLValue(identifiers_->identifiers());

    if (! ivalue)
      ivalue = js->lookupProperty(identifiers_->identifiers(), /*create*/true);

    //---

    // iterate on key values
    CJValueP value = inExpr_->exec(js);

    if      (value->hasIndex()) {
      int len = value->length();

      for (int ind = 0; ind < len; ++ind) {
        CJValueP value1(js->createNumberValue(long(ind)));

        ivalue->setValue(value1);

        if (block_) {
          js->startBlock(block_);

          block_->exec(js);

          bool breakFlag = block_->isBreakFlag();

          js->endBlock();

          if (breakFlag)
            break;
        }
      }
    }
    else if (value->hasProperty()) {
      for (auto &ind : value->propertyNames()) {
        CJValueP value1(js->createStringValue(ind));

        ivalue->setValue(value1);

        if (block_) {
          js->startBlock(block_);

          block_->exec(js);

          bool breakFlag = block_->isBreakFlag();

          js->endBlock();

          if (breakFlag)
            break;
        }
      }
    }
    else {
      js->errorMsg("Non-index value : for in ");
      return CJValueP();
    }

  }
  // for (<expr1>; <expr2>; <expr3>)
  // for (var <expr1>; <expr2>; <expr3>)
  else if (exprList2_ && exprList3_) {
    if      (exprList1_)
      (void) exprList1_->exec(js);
    else if (var_)
      (void) var_->exec(js);
    else
      return CJValueP();

    //---

    CJValueP value2 = exprList2_->exec(js);

    while (value2 && value2->toBoolean()) {
      if (block_) {
        js->startBlock(block_);

        block_->exec(js);

        bool breakFlag = block_->isBreakFlag();

        js->endBlock();

        if (breakFlag)
          break;
      }

      (void) exprList3_->exec(js);

      value2 = exprList2_->exec(js);
    }
  }

  return CJValueP();
}

void
CJExecFor::
print(std::ostream &os) const
{
  os << "for (";

  if (var_) {
    os << *var_;
  }
  else {
    if (exprList1_)
      os << *exprList1_;
  }

  if (inExpr_)
    os << " in " << *inExpr_;
  else {
    os << ";";

    if (exprList2_)
      os << " " << *exprList2_;

    os << ";";

    if (exprList3_)
      os << " " << *exprList3_;
  }

  os << ")";

  if (block_)
    os << " " << *block_;
}

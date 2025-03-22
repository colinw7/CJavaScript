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

    CJLValueP ivalue = js->lookupLValue(identifiers_);

    if (! ivalue)
      ivalue = js->lookupProperty(identifiers_, /*create*/true);

    //---

    // iterate on key values
    CJValueP value = inExpr_->exec(js);

    if (! value) {
      js->errorMsg("Invalid value for for in");
      return CJValueP();
    }

    // upgrade from primitive
    if (value->isPrimitive())
      value = js->valueToObject(value);

    // iterator array or property values
    if      (value->hasIndex()) {
      CJArrayP valueArray;

      if (value->isArray())
        valueArray = CJValue::cast<CJArray>(value);

      long len = value->length().value_or(0);

      for (int ind = 0; ind < len; ++ind) {
        if (valueArray && ! valueArray->isEnumerableIndex(ind))
          continue;

        CJValueP ivalue1 = value->indexValue(ind);

        if (js->isUndefinedValue(ivalue1))
          continue;

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

      if (valueArray) {
        for (auto &ind : valueArray->propertyNames()) {
          if (! valueArray->isEnumerableProperty(ind))
            continue;

          //CJValueP ivalue1 = valueArray->propertyValue(ind);

          //if (js->isUndefinedValue(ivalue1))
          //  continue;

          CJStringP value1 = js->createStringValue(ind);

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
    }
    else if (value->hasProperty()) {
      CJDictionaryP valueDict;

      if (value->isDictionary())
        valueDict = CJValue::cast<CJDictionary>(value);

      for (auto &ind : value->propertyNames()) {
        if (valueDict && ! valueDict->isEnumerableProperty(ind))
          continue;

        //CJValueP ivalue1 = value->propertyValue(ind);

        //if (js->isUndefinedValue(ivalue1))
        //  continue;

        CJStringP value1 = js->createStringValue(ind);

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

    bool flag2 = true;

    if (! exprList2_->isEmpty()) {
      CJValueP value2 = exprList2_->exec(js);

      flag2 = (value2 && value2->toBoolean());
    }

    while (flag2) {
      if (block_) {
        js->startBlock(block_);

        block_->exec(js);

        bool breakFlag = block_->isBreakFlag();

        js->endBlock();

        if (breakFlag)
          break;
      }

      //---

      (void) exprList3_->exec(js);

      //---

      flag2 = true;

      if (! exprList2_->isEmpty()) {
        CJValueP value2 = exprList2_->exec(js);

        flag2 = (value2 && value2->toBoolean());
      }
    }
  }

  return CJValueP();
}

std::string
CJExecFor::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
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

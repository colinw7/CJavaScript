#include <CJExecAssignExpression.h>
#include <CJExecIndexExpression.h>
#include <CJExecIdentifiers.h>
#include <CJavaScript.h>

CJExecAssignExpression::
CJExecAssignExpression() :
 CJExecExpression(CJToken::Type::AssignExpression)
{
}

CJValueP
CJExecAssignExpression::
exec(CJavaScript *js)
{
  CJValueP value;

  if (! lexpr_ || ! rexpr_)
    return value;

  const CJExecExpression::Tokens &tokens = lexpr_->tokens();

  if (tokens.size() == 0)
    return value;

  CJValueP llvalue;

  if (tokens.size() > 1) {
    int spos = -1;

    for (uint i = 0; i < tokens.size(); ++i) {
      CJTokenP token = lexpr_->token(i);

      if (token->type() == CJToken::Type::Operator) {
        CJOperator *op = token->cast<CJOperator>();

        if (op->type() == CJOperator::Type::Scope)
          spos = i;
      }
    }

    if (spos < 0) {
      js->errorMsg(this, "Missing scope for multiple token assign");
      return value;
    }

    if (spos != int(tokens.size()) - 2) {
      js->errorMsg(this, "Only one token allowed after scope for multiple token assign");
      return value;
    }

    CJExecExpressionP lexpr1(new CJExecExpression(lexpr_->type()));

    for (uint i = 0; i < tokens.size() - 2; ++i)
      lexpr1->addToken(lexpr_->token(i));

    llvalue = lexpr1->exec(js);

    if (! llvalue)
      return value;

    //llvalue->print(std::cerr);
  }

  //---

  CJTokenP varToken = tokens.back();

  CJExecIdentifiers *eidentifiers = 0;

  std::vector<CJExecExpressionP> indices;

  if      (varToken && varToken->type() == CJToken::Type::IndexExpression) {
    CJExecIndexExpression *iexpr = varToken->cast<CJExecIndexExpression>();

    CJExecIdentifiersP identifiers;

    if (! iexpr->decodeExpressions(identifiers, indices)) {
      js->errorMsg(this, "Missing variable name for assign");
      return value;
    }

    eidentifiers = identifiers.get();
  }
  else if (varToken && varToken->type() == CJToken::Type::Identifiers) {
    eidentifiers = varToken->cast<CJExecIdentifiers>();
  }

  //---

  if (! eidentifiers) {
    js->errorMsg(this, "Missing variable name for assign");
    return value;
  }

  //---

  const CJavaScript::Identifiers &identifiers = eidentifiers->identifiers();

  CJLValueP lvalue = js->lookupLValue(identifiers);

  if (llvalue) {
    CJPropertyData data;

    data.modifiable = true;

    if (! js->lookupPropertyData(llvalue, identifiers, data)) {
      js->errorMsg(this, "No property " + eidentifiers->toString() + " for value " +
                   llvalue->toString());
      return value;
    }

    if (! data.lvalue) {
      js->errorMsg(this, "Property " + eidentifiers->toString() + " for value " +
                   llvalue->toString() + " is not an lvalue");
      return value;
    }

    lvalue = data.lvalue;
  }
  else {
    lvalue = js->lookupLValue(identifiers);

    if (! lvalue)
      lvalue = js->lookupProperty(identifiers, /*create*/true);
  }

  //---

  CJValueP rvalue = rexpr_->exec(js);

  if (! rvalue)
    return value;

  CJValueP varValue;

  if (lvalue)
    varValue = lvalue->value();

  if (! varValue)
    varValue = js->createNumberValue(long(0));

  CJOperator::Type opType = op_->type();

  switch (opType) {
    case CJOperator::Type::Assign:
      break;
    case CJOperator::Type::PlusAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Plus, varValue, rvalue);
      break;
    case CJOperator::Type::MinusAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Minus, varValue, rvalue);
      break;
    case CJOperator::Type::TimesAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Times, varValue, rvalue);
      break;
    case CJOperator::Type::DivideAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Divide, varValue, rvalue);
      break;
    case CJOperator::Type::ModulusAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Modulus, varValue, rvalue);
      break;
    case CJOperator::Type::LeftShiftAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::LeftShift, varValue, rvalue);
      break;
    case CJOperator::Type::RightShiftAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::RightShift, varValue, rvalue);
      break;
    case CJOperator::Type::UnsignedRightShiftAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::UnsignedRightShift, varValue, rvalue);
      break;
    case CJOperator::Type::BitwiseAndAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseAnd, varValue, rvalue);
      break;
    case CJOperator::Type::BitwiseOrAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseOr, varValue, rvalue);
      break;
    case CJOperator::Type::BitwiseNotAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseNot, varValue, rvalue);
      break;
    case CJOperator::Type::BitwiseXorAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseXor, varValue, rvalue);
      break;
    default:
      assert(false);
      break;
  }

  if (! indices.empty()) {
    std::vector<long> inds;

    for (auto indexExpr : indices) {
      CJValueP ivalue = indexExpr->exec(js);

      if (! ivalue) {
        js->errorMsg(this, "Invalid array index expression '" + indexExpr->toString() + "'");
        return value;
      }

      long ind = ivalue->toInteger();

      inds.push_back(ind);
    }

    for (uint i = 0; i < inds.size(); ++i) {
      int ind = inds[i];

      // last index
      if (i == inds.size() - 1) {
        if      (varValue->hasIndex()) {
          varValue->setIndexValue(ind, rvalue);
        }
        else if (varValue->hasProperty()) {
          CJValueP ivalue = js->createNumberValue(long(ind));

          std::string ind = ivalue->toString();

          varValue->setPropertyValue(ind, rvalue);
        }
        else {
          js->errorMsg(this, "Variable is not an array or dictionary");
          return value;
        }
      }
      // non last index
      else {
        CJValueP varValue1;

        if      (varValue->hasIndex()) {
          varValue1 = varValue->indexValue(ind);
        }
        else if (varValue->hasProperty()) {
          CJValueP ivalue = js->createNumberValue(long(ind));

          std::string ind = ivalue->toString();

          varValue1 = varValue->propertyValue(ind);
        }

        if (! varValue1) {
          js->errorMsg(this, "Variable is not an array or dictionary");
          return value;
        }

        varValue = varValue1;
      }
    }
  }
  else {
    if (lvalue)
      lvalue->setValue(rvalue);
  }

  return rvalue;
}

void
CJExecAssignExpression::
print(std::ostream &os) const
{
  if (lexpr_ && op_ && rexpr_) {
    os << *lexpr_ << " " << *op_ << " " << *rexpr_ << ";";
  }
}

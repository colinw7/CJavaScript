#include <CJExecAssignExpression.h>
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
  CJExecExpression  *indexExpr = 0;

  if      (varToken && varToken->type() == CJToken::Type::IndexExpression) {
    eidentifiers = varToken->cast<CJExecIndexExpression>()->identifiers().get();
    indexExpr    = varToken->cast<CJExecIndexExpression>()->indexExpression().get();
  }
  else if (varToken && varToken->type() == CJToken::Type::Identifiers) {
    eidentifiers = varToken->cast<CJExecIdentifiers>();
  }

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

  if (indexExpr) {
    CJValueP ivalue = indexExpr->exec(js);

    long ind = ivalue->toInteger();

    if      (varValue->type() == CJToken::Type::String) {
      CJString *str = varValue->cast<CJString>();

      str->setIndexValue(ind, rvalue);
    }
    else if (varValue->hasIndex()) {
      varValue->setIndexValue(ind, rvalue);
    }
    else {
      js->errorMsg(this, "Variable is not an array");
      return value;
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
    os << *lexpr_ << " " << *op_ << " " << *rexpr_;
  }
}

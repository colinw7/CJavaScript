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

  //---

  std::vector<CJValueP> ivalues;

  for (auto indexExpr : indices) {
    CJValueP ivalue = indexExpr->exec(js);

    if (! ivalue) {
      js->errorMsg(this, "Invalid array index expression '" + indexExpr->toString() + "'");
      return value;
    }

    ivalues.push_back(ivalue);
  }

  //---

  CJOperator::Type opType = op_->type();

  CJValueP rvarValue;

  // get current value for +=, -=, ...
  if (opType != CJOperator::Type::Assign) {
    if (! ivalues.empty()) {
      rvarValue = getIndexValue(varValue, ivalues);

      if (! rvarValue) {
        js->errorMsg(this, "Variable is not an array or dictionary");
        return CJValueP();
      }
    }
    else
      rvarValue = varValue;
  }

  //---

  switch (opType) {
    case CJOperator::Type::Assign:
      break;
    case CJOperator::Type::PlusAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Plus, rvarValue, rvalue);
      break;
    case CJOperator::Type::MinusAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Minus, rvarValue, rvalue);
      break;
    case CJOperator::Type::TimesAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Times, rvarValue, rvalue);
      break;
    case CJOperator::Type::DivideAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Divide, rvarValue, rvalue);
      break;
    case CJOperator::Type::ModulusAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Modulus, rvarValue, rvalue);
      break;
    case CJOperator::Type::LeftShiftAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::LeftShift, rvarValue, rvalue);
      break;
    case CJOperator::Type::RightShiftAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::RightShift, rvarValue, rvalue);
      break;
    case CJOperator::Type::UnsignedRightShiftAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::UnsignedRightShift, rvarValue, rvalue);
      break;
    case CJOperator::Type::BitwiseAndAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseAnd, rvarValue, rvalue);
      break;
    case CJOperator::Type::BitwiseOrAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseOr, rvarValue, rvalue);
      break;
    case CJOperator::Type::BitwiseNotAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseNot, rvarValue, rvalue);
      break;
    case CJOperator::Type::BitwiseXorAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseXor, rvarValue, rvalue);
      break;
    default:
      assert(false);
      break;
  }

  if (! indices.empty()) {
    if (! setIndexValue(varValue, ivalues, rvalue)) {
      js->errorMsg(this, "Variable is not an array or dictionary");
      return CJValueP();
    }
  }
  else {
    if (lvalue)
      lvalue->setValue(rvalue);
  }

  return rvalue;
}

CJValueP
CJExecAssignExpression::
getIndexValue(CJValueP varValue, const std::vector<CJValueP> &ivalues)
{
  CJValueP varValue1 = varValue;

  for (uint i = 0; i < ivalues.size(); ++i) {
    CJValueP ivalue = ivalues[i];

    CJValueP varValue2;

    if      (varValue1->hasIndex()) {
      long ind = ivalue->toInteger();

      varValue2 = varValue1->indexValue(ind);
    }
    else if (varValue1->hasProperty()) {
      std::string ind = ivalue->toString();

      varValue2 = varValue1->propertyValue(ind);
    }
    else {
      return CJValueP();
    }

    if (! varValue2)
      return CJValueP();

    varValue1 = varValue2;
  }

  return varValue1;
}

bool
CJExecAssignExpression::
setIndexValue(CJValueP varValue, const std::vector<CJValueP> &ivalues, CJValueP rvalue)
{
  CJValueP varValue1 = varValue;

  for (uint i = 0; i < ivalues.size(); ++i) {
    CJValueP ivalue = ivalues[i];

    if      (varValue1->hasIndex()) {
      long ind = ivalue->toInteger();

      // last index
      if (i == ivalues.size() - 1)
        varValue1->setIndexValue(ind, rvalue);
      // non last index
      else
        varValue1 = varValue1->indexValue(ind);
    }
    else if (varValue1->hasProperty()) {
      std::string ind = ivalue->toString();

      // last index
      if (i == ivalues.size() - 1)
        varValue1->setPropertyValue(ind, rvalue);
      // non last index
      else
        varValue1 = varValue1->propertyValue(ind);
    }
    else {
      return false;
    }
  }

  return true;
}

void
CJExecAssignExpression::
print(std::ostream &os) const
{
  if (lexpr_ && op_ && rexpr_) {
    os << *lexpr_ << " " << *op_ << " " << *rexpr_ << ";";
  }
}

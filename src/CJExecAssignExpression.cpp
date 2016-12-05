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
        CJOperator *op = token->castP<CJOperator>();

        if (op->type() == CJOperator::Type::Scope)
          spos = i;
      }
    }

    if (spos < 0) {
      js->throwTypeError(this, "Missing scope for multiple token assign");
      return value;
    }

    if (spos != int(tokens.size()) - 2) {
      js->throwTypeError(this, "Only one token allowed after scope for multiple token assign");
      return value;
    }

    CJExecExpressionP lexpr1 = js->createExecExpression(lexpr_->type());

    for (uint i = 0; i < tokens.size() - 2; ++i)
      lexpr1->addToken(lexpr_->token(i));

    llvalue = lexpr1->exec(js);

    if (! llvalue)
      return value;

    //llvalue->print(std::cerr);
  }

  //---

  CJTokenP varToken = tokens.back();

  CJExecIdentifiersP eidentifiers;

  std::vector<CJExecExpressionP> indices;

  if      (varToken && varToken->type() == CJToken::Type::IndexExpression) {
    CJExecIndexExpression *iexpr = varToken->castP<CJExecIndexExpression>();

    CJExecIdentifiersP eidentifiers1;

    if (! iexpr->decodeExpressions(eidentifiers1, indices)) {
      js->throwTypeError(this, "Missing variable name for assign");
      return value;
    }

    eidentifiers = eidentifiers1;
  }
  else if (varToken && varToken->type() == CJToken::Type::Identifiers) {
    eidentifiers = CJToken::cast<CJExecIdentifiers>(varToken);
  }

  //---

  if (! eidentifiers) {
    js->throwTypeError(this, "Missing variable name for assign");
    return value;
  }

  //---

  CJLValueP lvalue;

  //lvalue = js->lookupLValue(eidentifiers);

  if (llvalue) {
    CJPropertyData data(js);

    data.setModifiable(true);

    if (! js->lookupValuePropertyData(llvalue, eidentifiers, data)) {
      js->throwTypeError(this, "No property " + eidentifiers->toString() + " for value " +
                         llvalue->toString());
      return value;
    }

    lvalue = data.lvalue();

    if (! lvalue) {
      js->throwTypeError(this, "Property " + eidentifiers->toString() + " for value " +
                         llvalue->toString() + " is not an lvalue");
      return value;
    }
  }
  else {
    lvalue = js->lookupAssignLValue(eidentifiers);

    if (! lvalue) {
      if (! js->isStrict()) {
        // create variable in global scope if not found
        CJDictionaryP scope = js->rootScope();

        if (eidentifiers->isThis())
          scope = js->thisScope();
        else
          scope = js->rootScope();

        CJPropertyData data(js);

        data.setModifiable(true);
        data.setCreate    (true);

        if (js->lookupScopePropertyData(scope, eidentifiers, data))
          lvalue = data.lvalue();

        //lvalue = js->lookupScopeProperty(scope, eidentifiers, /*create*/true);

        //lvalue = js->lookupProperty(eidentifiers, /*create*/true);
      }
    }

    if (! lvalue) {
      js->throwTypeError(this, "Variable " + eidentifiers->toString() + " does not exist");
      return CJValueP();
    }
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
      js->throwTypeError(this, "Invalid array index expression '" +
                         indexExpr->toString() + "'");
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
      rvarValue = getIndexValue(js, varValue, ivalues);

      if (! rvarValue) {
        js->throwTypeError(this, "Variable is not an array or dictionary");
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
    if (! setIndexValue(js, varValue, ivalues, rvalue)) {
      js->throwTypeError(this, "Variable is not an array or dictionary");
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
getIndexValue(CJavaScript *js, CJValueP varValue, const Values &ivalues)
{
  CJValueP varValue1 = varValue;

  for (uint i = 0; i < ivalues.size(); ++i) {
    CJValueP ivalue = ivalues[i];

    CJValueP varValue2;

    if (! js->indexValue(varValue1, ivalue, varValue2)) {
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
setIndexValue(CJavaScript *js, CJValueP varValue, const Values &ivalues, CJValueP rvalue)
{
  CJValueP varValue1 = varValue;

  for (uint i = 0; i < ivalues.size(); ++i) {
    CJValueP ivalue = ivalues[i];

    // last index
    if (i == ivalues.size() - 1) {
      if (! js->setIndexValue(varValue1, ivalue, rvalue))
        return false;
    }
    // non last index
    else {
      CJValueP rvalue1;

      if (! js->indexValue(varValue1, ivalue, rvalue1))
        return false;

      varValue1 = rvalue1;
    }
  }

  return true;
}

std::string
CJExecAssignExpression::
toString() const
{
  std::string str;

  if (lexpr_ && op_ && rexpr_) {
    str += lexpr_->toString() + " " + op_->toString() + " " + rexpr_->toString();
  }

  return str;
}

void
CJExecAssignExpression::
print(std::ostream &os) const
{
  if (lexpr_ && op_ && rexpr_) {
    os << *lexpr_ << " " << *op_ << " " << *rexpr_;
  }
}

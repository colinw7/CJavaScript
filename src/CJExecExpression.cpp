#include <CJExecExpression.h>
#include <CJavaScript.h>

CJValueP
CJExecExpression::
exec(CJavaScript *js)
{
  Values    values;
  Operators operators;

  int len = tokens_.size();

  for (int i = 0; i < len; ++i) {
    CJTokenP token = tokens_[i];

    if (! token)
      return CJValueP();

    CJToken::Type type = token->type();

    if      (token->isValue()) {
      CJValueP value = std::static_pointer_cast<CJValue>(token);

      values.push_back(value);
    }
    else if (type == CJToken::Type::Operator) {
      CJOperator *op = token->cast<CJOperator>();

      bool unstack = false;

      CJOperator *lastOp = operators.lastOp();

      if (lastOp) {
        if      (lastOp->precedence() < op->precedence())
          unstack = true;
        else if (lastOp->precedence() == op->precedence())
          unstack = (lastOp->associativty() == CJOperator::Associativty::Left);
      }

      if (unstack) {
        operators.pop_back();

        CJValueP value;

        if (lastOp->ary() == CJOperator::Ary::Binary) {
          if (values.size() < 2) {
            js->errorMsg("Missing values for binary op");
            return CJValueP();
          }

          CJValueP value2 = values.back(); values.pop_back();
          CJValueP value1 = values.back(); values.pop_back();

          value = js->execBinaryOp(lastOp->type(), value1, value2);

          if (value)
            values.push_back(value);
          else
            js->errorMsg("Invalid binary operator value : " +
                         (value1 ? value1->toString() : "<null>") + " " + lastOp->name() + " " +
                         (value2 ? value2->toString() : "<null>"));
        }
        else {
          if (values.size() < 1) {
            js->errorMsg("Missing value for unary op");
            return CJValueP();
          }

          CJValueP value1 = values.back(); values.pop_back();

          value = js->execUnaryOp(lastOp->type(), value1);

          if (value)
            values.push_back(value);
          else
            js->errorMsg("Invalid unary operator value : " +
                         lastOp->name() + " " + value1->toString());
        }
      }

      operators.push_back(op);
    }
    else if (type == CJToken::Type::Identifiers) {
      CJExecIdentifiers *identifiers = token->cast<CJExecIdentifiers>();

      CJOperator *lastOp = operators.lastOp();

      if (lastOp && lastOp->type() == CJOperator::Type::Scope) {
        if (values.empty()) {
          js->errorMsg("Missing value for identifiers scope");
          continue;
        }

        CJValueP value = values.back();

        values.pop_back();

        operators.pop_back();

        identifiers->setEValue(value);

        CJValueP res = identifiers->exec(js);

        values.push_back(res);
      }
      else {
        CJValueP value = identifiers->exec(js);

        //if (! value) {
        //  std::stringstream ss; ss << *identifiers;
        //  js->errorMsg("Invalid named value : " + ss.str());
        //}

        values.push_back(value);
      }
    }
    else if (type == CJToken::Type::Function) {
      if (isShortCircuit(operators, values)) {
        bool b = values[0]->toBoolean();

        return CJValueP(js->createBoolValue(b));
      }

      //---

      CJExecFunction *fn = token->cast<CJExecFunction>();

      CJOperator *lastOp = operators.lastOp();

      if (lastOp && lastOp->type() == CJOperator::Type::Scope) {
        if (values.empty()) {
          js->errorMsg("Missing value for function scope");
          continue;
        }

        CJValueP value = values.back();

        values.pop_back();

        operators.pop_back();

        fn->setEValue(value);

        CJValueP res = fn->exec(js);

        values.push_back(res);
      }
      else {
        CJValueP res = fn->exec(js);

        values.push_back(res);
      }
    }
    else if (type == CJToken::Type::IndexExpression) {
      CJExecIndexExpression *iexec = token->cast<CJExecIndexExpression>();

      if (! iexec->isBound()) {
        if (values.empty()) {
          js->errorMsg("Missing value for index ");
          continue;
        }

        CJValueP value = values.back();

        values.pop_back();

        iexec->setEValue(value);

        CJValueP res = iexec->exec(js);

        values.push_back(res);
      }
      else {
        CJValueP value = token->exec(js);

        values.push_back(value);
      }
    }
    else {
      if (isShortCircuit(operators, values)) {
        bool b = values[0]->toBoolean();

        return CJValueP(js->createBoolValue(b));
      }

      CJValueP value = token->exec(js);

      values.push_back(value);
    }
  }

  while (! operators.empty()) {
    CJOperator *lastOp = operators.lastOp();

    operators.pop_back();

    CJValueP value;

    if (lastOp->ary() == CJOperator::Ary::Binary) {
      if (values.size() < 2) {
        js->errorMsg("Missing values for binary op");
        return CJValueP();
      }

      CJValueP value2 = values.back(); values.pop_back();
      CJValueP value1 = values.back(); values.pop_back();

      value = js->execBinaryOp(lastOp->type(), value1, value2);

      if (value)
        values.push_back(value);
      else
        js->errorMsg("Invalid binary operator value : " +
                     (value1 ? value1->toString() : "<null>") + " " + lastOp->name() + " " +
                     (value2 ? value2->toString() : "<null>"));
    }
    else {
      if (values.size() < 1) {
        js->errorMsg("Missing value for unary op");
        return CJValueP();
      }

      CJValueP value1 = values.back(); values.pop_back();

      value = js->execUnaryOp(lastOp->type(), value1);

      if (value)
        values.push_back(value);
      else
        js->errorMsg("Invalid unary operator value : " +
                     lastOp->name() + " " + value1->toString());
    }
  }

  if (values.empty())
    return CJValueP();

  return values.back();
}

bool
CJExecExpression::
isShortCircuit(const Operators &operators, const Values &values) const
{
  CJOperator *lastOp = operators.lastOp();

  if (values.size() == 1 && operators.size() == 1 &&
      (lastOp->type() == CJOperator::Type::LogicalAnd ||
       lastOp->type() == CJOperator::Type::LogicalOr)) {
    bool shortCircuit = false;

    bool b = values[0]->toBoolean();

    if      (! b && lastOp->type() == CJOperator::Type::LogicalAnd)
      shortCircuit = true;
    else if (  b && lastOp->type() == CJOperator::Type::LogicalOr)
      shortCircuit = true;

    return shortCircuit;
  }
  else
    return false;
}

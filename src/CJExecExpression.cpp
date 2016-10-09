#include <CJExecExpression.h>
#include <CJExecIdentifiers.h>
#include <CJExecFunction.h>
#include <CJExecIndexExpression.h>
#include <CJTokenValue.h>
#include <CJavaScript.h>

namespace {
  std::string valueToString(CJValueP value) {
    return (value ? value->toString() : "<null>");
  }
}

//---

CJValueP
CJExecExpression::
exec(CJavaScript *js)
{
  class State {
   public:
    State(CJavaScript *js) :
     js_(js) {
    }

    void pushValue(CJExecIdentifiers *identifiers) {
      CJValueP value = identifiers->exec(js_);

      //if (! value) {
      //  std::stringstream ss; ss << *identifiers;
      //  js->errorMsg("Invalid named value : " + ss.str());
      //}

      if (js_->isExprDebug()) {
        if (value)
          std::cerr << "pushValue " << *identifiers << "=" << *value << std::endl;
        else
          std::cerr << "pushValue " << *identifiers << "=<null>" << std::endl;
      }

      //if (! value)
      //  value = js_->createUndefinedValue();

      values_.push_back(value);
    }

    void pushValue(CJValueP value) {
      if (js_->isExprDebug()) {
        if (value)
          std::cerr << "pushValue " << *value << std::endl;
        else
          std::cerr << "pushValue <null>" << std::endl;
      }

      values_.push_back(value);
    }

    void pushTokenValue(CJTokenP token, CJValueP value) {
      if (js_->isExprDebug()) {
        if (value)
          std::cerr << "pushTokenValue " << *token << "(" << *value << ")" << std::endl;
        else
          std::cerr << "pushTokenValue " << *token << "(<null>>" << std::endl;
      }

      CJTokenValueP tvalue(new CJTokenValue(js_));

      tvalue->addToken(token);
      tvalue->setValue(value);

      values_.push_back(tvalue);
    }

    CJValueP popValue() {
      if (js_->isExprDebug())
        std::cerr << "popValue" << std::endl;

      CJValueP value = values_.back();

      values_.pop_back();

      return value;
    }

    CJValueP lastValue() {
      if (values_.empty())
        return CJValueP();

      return values_.back();
    }

    bool anyValues() {
      return ! values_.empty();
    }

    int numValues() {
      return values_.size();
    }

    void pushOperator(CJOperator *op) {
      if (js_->isExprDebug())
        std::cerr << "pushOperator " << *op << std::endl;

       operators_.push_back(op);
     }

     void popOperator() {
      if (js_->isExprDebug())
        std::cerr << "popOperator" << std::endl;

       operators_.pop_back();
     }

     bool anyOperators() {
       return ! operators_.empty();
     }

     CJOperator *lastOp() {
       return operators_.lastOp();
     }

     bool isShortCircuit() {
       return CJExecExpression::isShortCircuit(operators_, values_);
     }

   private:
    CJavaScript *js_;
    Values       values_;
    Operators    operators_;
  };

  //---

  State state(js);

  if (js->isExprDebug())
    std::cerr << *this << std::endl;

  int len = tokens_.size();

  for (int i = 0; i < len; ++i) {
    CJTokenP token = tokens_[i];

    if (! token)
      return CJValueP();

    CJOperator *lastOp  = state.lastOp();

    CJToken::Type type = token->type();

    if      (token->isValue()) {
      CJValueP value = std::static_pointer_cast<CJValue>(token);

      if (lastOp && lastOp->type() == CJOperator::Type::Delete)
        state.pushTokenValue(token, value);
      else
        state.pushValue(value);
    }
    else if (type == CJToken::Type::Operator) {
      CJOperator *op = token->castP<CJOperator>();

      bool unstack = false;

      if (lastOp) {
        if      (lastOp->precedence() < op->precedence())
          unstack = true;
        else if (lastOp->precedence() == op->precedence())
          unstack = (lastOp->associativty() == CJOperator::Associativty::Left);
      }

      while (unstack) {
        state.popOperator();

        CJValueP value;

        if (lastOp->ary() == CJOperator::Ary::Binary) {
          if (state.numValues() < 2) {
            js->errorMsg(this, "Missing values for binary op");
            return CJValueP();
          }

          CJValueP value2 = state.popValue();
          CJValueP value1 = state.popValue();

          value = js->execBinaryOp(lastOp->type(), value1, value2);

          if (value)
            state.pushValue(value);
          else
            js->errorMsg(this, "Invalid binary operator value : " +
                         valueToString(value1) + " " + lastOp->name() + " " +
                         valueToString(value2));
        }
        else {
          if (state.numValues() < 1) {
            js->errorMsg(this, "Missing value for unary op");
            return CJValueP();
          }

          CJValueP value1 = state.popValue();

          value = js->execUnaryOp(lastOp->type(), value1);

          if (value)
            state.pushValue(value);
          else
            js->errorMsg(this, "Invalid unary operator value : " +
                         lastOp->name() + " " + valueToString(value1));
        }

        //---

        unstack = false;
        lastOp  = state.lastOp();

        if (lastOp) {
          if      (lastOp->precedence() < op->precedence())
            unstack = true;
          else if (lastOp->precedence() == op->precedence())
            unstack = (lastOp->associativty() == CJOperator::Associativty::Left);
        }
      }

      //---

      state.pushOperator(op);
    }
    else if (type == CJToken::Type::Identifiers) {
      CJExecIdentifiers *identifiers = token->castP<CJExecIdentifiers>();

      if      (lastOp && lastOp->type() == CJOperator::Type::Scope) {
        if (! state.anyValues()) {
          js->errorMsg(this, "Missing value for identifiers scope");
          continue;
        }

        CJValueP value = state.popValue();

        state.popOperator();

        identifiers->setEValue(value);

        state.pushValue(identifiers);
      }
      else {
        if (lastOp && lastOp->type() == CJOperator::Type::Delete) {
          CJValueP value = identifiers->exec(js);

          state.pushTokenValue(token, value);
        }
        else {
          state.pushValue(identifiers);
        }
      }
    }
    else if (type == CJToken::Type::ExecFunction) {
      if (state.isShortCircuit()) {
        CJValueP value = state.popValue();

#if 0
        bool b = value->toBoolean();

        return js->createBoolValue(b);
#else
        return value;
#endif
      }

      //---

      CJExecFunction *fn = token->castP<CJExecFunction>();

      if (lastOp && lastOp->type() == CJOperator::Type::Scope) {
        if (! state.anyValues()) {
          js->errorMsg(this, "Missing value for function scope");
          continue;
        }

        CJValueP value = state.popValue();

        state.popOperator();

        fn->setEValue(value);

        CJValueP res = fn->exec(js);

        state.pushValue(res);
      }
      else {
        CJValueP res = fn->exec(js);

        state.pushValue(res);
      }
    }
    else if (type == CJToken::Type::IndexExpression) {
      CJExecIndexExpression *iexec = token->castP<CJExecIndexExpression>();

      if (! iexec->isBound()) {
        if (! state.anyValues()) {
          js->errorMsg(this, "Missing value for index ");
          continue;
        }

        CJValueP value = state.popValue();

        iexec->setEValue(value);

        CJValueP res = iexec->exec(js);

        state.pushValue(res);
      }
      else {
        CJValueP value = token->exec(js);

        if (lastOp && lastOp->type() == CJOperator::Type::Delete)
          state.pushTokenValue(token, value);
        else
          state.pushValue(value);
      }
    }
    else {
      if (state.isShortCircuit()) {
        CJValueP value = state.popValue();

#if 0
        bool b = value->toBoolean();

        return js->createBoolValue(b);
#else
        return value;
#endif
      }

      CJValueP value = token->exec(js);

      if (lastOp && lastOp->type() == CJOperator::Type::Delete)
        state.pushTokenValue(token, value);
      else
        state.pushValue(value);
    }
  }

  while (state.anyOperators()) {
    CJOperator *lastOp = state.lastOp();

    state.popOperator();

    CJValueP value;

    if (lastOp->ary() == CJOperator::Ary::Binary) {
      if (state.numValues() < 2) {
        js->errorMsg(this, "Missing values for binary op");
        return CJValueP();
      }

      CJValueP value2 = state.popValue();
      CJValueP value1 = state.popValue();

      value = js->execBinaryOp(lastOp->type(), value1, value2);

      if (value)
        state.pushValue(value);
      else
        js->errorMsg(this, "Invalid binary operator value : " +
                     valueToString(value1) + " " + lastOp->name() + " " +
                     valueToString(value2));
    }
    else {
      if (state.numValues() < 1) {
        js->errorMsg(this, "Missing value for unary op");
        return CJValueP();
      }

      CJValueP value1 = state.popValue();

      value = js->execUnaryOp(lastOp->type(), value1);

      if (value)
        state.pushValue(value);
      else
        js->errorMsg(this, "Invalid unary operator value : " +
                     lastOp->name() + " " + valueToString(value1));
    }
  }

  if (! state.anyValues())
    return CJValueP();

  return state.popValue();
}

bool
CJExecExpression::
isShortCircuit(const Operators &operators, const Values &values)
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

std::string
CJExecExpression::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJExecExpression::
print(std::ostream &os) const
{
  int i = 0;

  for (auto &t : tokens_) {
    if (i > 0)
      os << " ";

    if (t)
      os << *t;
    else
      os << "null";

    ++i;
  }
}

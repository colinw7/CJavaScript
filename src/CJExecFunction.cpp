#include <CJExecFunction.h>
#include <CJExecExpression.h>
#include <CJExecIdentifiers.h>
#include <CJExecExpressionList.h>
#include <CJavaScript.h>

CJExecFunction::
CJExecFunction() :
 CJToken(CJToken::Type::ExecFunction)
{
}

CJValueP
CJExecFunction::
exec(CJavaScript *js)
{
  typedef std::vector<CJValueP> Values;

  Values values;

  CJFunctionP fn;
  CJObjP      obj;

  if (expr_ || evalue_) {
    CJValueP evalue;

    if (expr_)
      evalue = expr_->exec(js);
    else
      evalue = evalue_;

    if (! evalue) {
      js->errorMsg(this, "Missing function name");
      return CJValueP();
    }

    if      (identifiers_) {
      auto identifiers = identifiers_->identifiers();

      if (identifiers.size() != 1) {
        std::stringstream ss; ss << *identifiers_;
        js->errorMsg(identifiers_, "Invalid type function identifiers : " + ss.str());
        return CJValueP();
      }

      CJValueP fnValue;

      CJPropertyData data;

      if (js->lookupPropertyData(evalue, identifiers, data, 0)) {
        fnValue = data.value;
      }

      if (! fnValue) {
        std::stringstream ss; ss << *identifiers_;
        js->errorMsg(identifiers_, "Function not found : " + ss.str());
        return CJValueP();
      }

      fn = js->valueToFunction(fnValue);

      if (! fn) {
        std::stringstream ss; ss << *identifiers_;
        js->errorMsg(fnValue, "Function not found : " + ss.str());
        return CJValueP();
      }

      if (fn->hasObjectValue())
        values.push_back(evalue);
    }
    else {
      fn = js->valueToFunction(evalue);

      if (! fn) {
        js->errorMsg(this, "Missing function name");
        return CJValueP();
      }

      if (fn->hasObjectValue())
        values.push_back(evalue);
    }
  }
  else {
    if (! identifiers_) {
      js->errorMsg(this, "Missing function name");
      return CJValueP();
    }

    auto identifiers = identifiers_->identifiers();

    CJavaScript::ValuePair valuePair = js->lookupObjectProperty(identifiers);

    if (valuePair.first && valuePair.first->isObject())
      obj = std::static_pointer_cast<CJObj>(valuePair.first);

    CJValueP fnValue = valuePair.second;

    if (! fnValue) {
      std::stringstream ss; ss << *identifiers_;
      js->errorMsg(identifiers_, "Function not found : " + ss.str());
      return CJValueP();
    }

    fn = js->valueToFunction(fnValue);

    if (! fn) {
      std::stringstream ss; ss << *fnValue;
      js->errorMsg(fnValue, "Invalid function value : " + ss.str());
      return CJValueP();
    }

    if (fn->hasObjectValue())
      values.push_back(valuePair.first);
  }

  //--

  CJExecExpressionList::Values values1;

  if (exprList_)
    values1 = exprList_->getValues(js);

  for (auto &v1 : values1)
    values.push_back(v1);

  if (! fn) {
    std::stringstream ss; ss << *identifiers_;
    js->errorMsg(this, "Function not found : " + ss.str());
    return CJValueP();
  }

  CJValueP res;

  if (obj) {
    js->pushThis(obj);

    res = fn->cast<CJFunction>()->exec(js, values);

    js->popThis();
  }
  else
    res = fn->cast<CJFunction>()->exec(js, values);

  //--

  for (auto &fn : functions_) {
    fn->setEValue(res);

    res = fn->exec(js);
  }

  return res;
}

void
CJExecFunction::
print(std::ostream &os) const
{
  if      (identifiers_) {
    if (exprList_)
      os << *identifiers_ << "(" << *exprList_ << ")";
    else
      os << *identifiers_ << "()";
  }
  else if (expr_) {
    if (exprList_)
      os << *expr_ << "(" << *exprList_ << ")";
    else
      os << *expr_ << "()";
  }
}

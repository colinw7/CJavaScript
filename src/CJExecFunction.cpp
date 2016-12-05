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

  CJFunctionBaseP fn;
  CJObjP          obj;

  if (expr_ || evalue_) {
    CJValueP evalue;

    if (expr_)
      evalue = expr_->exec(js);
    else
      evalue = evalue_;

    if (! evalue) {
      js->throwSyntaxError(this, "Missing function name");
      return CJValueP();
    }

    if      (identifiers_) {
      assert(! identifiers_->isThis());

      if (identifiers_->numIdentifiers() != 1) {
        std::stringstream ss; ss << *identifiers_;
        js->throwSyntaxError(this, "Invalid type function identifiers : " + ss.str());
        return CJValueP();
      }

      CJValueP evalue1;

      if (! evalue->isObject())
        evalue1 = js->valueToObj(evalue);
      else
        evalue1 = evalue;

      CJValueP fnValue;

      CJPropertyData data(js);

      if (evalue1 && js->lookupValuePropertyData(evalue1, identifiers_, data, 0)) {
        fnValue = data.value();
      }

      if (! fnValue) {
        std::stringstream ss; ss << *identifiers_;
        js->throwSyntaxError(this, "Function not found : " + ss.str());
        return CJValueP();
      }

      fn = js->valueToFunction(fnValue);

      if (! fn) {
        std::stringstream ss; ss << *identifiers_;
        js->throwSyntaxError(this, "Function not found : " + ss.str());
        return CJValueP();
      }

      if (fn->hasObjectValue())
        values.push_back(evalue1);
    }
    else {
      fn = js->valueToFunction(evalue);

      if (! fn) {
        js->throwSyntaxError(this, "Missing function name");
        return CJValueP();
      }

      if (fn->hasObjectValue())
        values.push_back(evalue);
    }
  }
  else {
    if (! identifiers_) {
      js->throwSyntaxError(this, "Missing function name");
      return CJValueP();
    }

    CJavaScript::ValuePair valuePair = js->lookupObjectProperty(identifiers_);

    if (valuePair.first && valuePair.first->isObject())
      obj = CJValue::cast<CJObj>(valuePair.first);

    CJValueP fnValue = valuePair.second;

    if (! fnValue) {
      std::stringstream ss; ss << *identifiers_;
      js->throwReferenceError(this, ss.str() + " is not defined");
      return CJValueP();
    }

    fn = js->valueToFunction(fnValue);

    if (! fn) {
      std::stringstream ss; ss << *fnValue;
      js->throwSyntaxError(this, "Invalid function value : " + ss.str());
      return CJValueP();
    }

    if (fn->hasObjectValue())
      values.push_back(valuePair.first);
  }

  //--

  CJExecExpressionList::Values values1;

  if (exprList_)
    values1 = exprList_->getValues(js);

  for (auto &v1 : values1) {
    if (! v1) {
      js->throwReferenceError(this, "variable not defined");
      return CJValueP();
    }

    values.push_back(v1);
  }

  if (! fn) {
    std::stringstream ss; ss << *identifiers_;
    js->throwSyntaxError(this, "Function not found : " + ss.str());
    return CJValueP();
  }

  CJDictionaryP thisDict = obj;

  if (! thisDict) {
    if (values.size() > 0 && values[0] && values[0]->isDictionary())
      thisDict = CJValue::cast<CJDictionary>(values[0]);
  }

  CJValueP res;

  if (thisDict) {
    js->pushThis(thisDict);

    res = fn->exec(js, values);

    js->popThis();
  }
  else {
    res = fn->exec(js, values);
  }

  //--

  for (auto &fn : functions_) {
    fn->setEValue(res);

    res = fn->exec(js);
  }

  //--

  return res;
}

std::string
CJExecFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
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

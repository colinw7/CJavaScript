#include <CJExecThis.h>
#include <CJExecIdentifiers.h>
#include <CJExecExpression.h>
#include <CJExecIndexExpression.h>
#include <CJavaScript.h>

CJExecThis::
CJExecThis() :
 CJToken(CJToken::Type::This)
{
}

CJValueP
CJExecThis::
exec(CJavaScript *js)
{
  CJDictionaryP dict = js->thisScope();

  if      (identifiers_) {
    const CJavaScript::Identifiers &identifiers = identifiers_->identifiers();

    CJPropertyData data;

    if (assign_ || incrDecr_)
      data.create = true;

    data.modifiable = true;

    if (! js->lookupPropertyData(dict, identifiers, data))
      return CJValueP();

    if      (assign_) {
      CJValueP avalue = assign_->exec(js);

      if (data.lvalue)
        data.lvalue->setValue(avalue);
      else
        js->errorMsg("Invalid assign for this");

      return avalue;
    }
    else if (incrDecr_) {
      if (data.value->type() == CJValue::Type::Null)
        data.value = js->createNumberValue(0L);

      CJValueP value1 = js->execUnaryOp(incrDecr_->type(), data.value);

      if (data.lvalue)
        data.lvalue->setValue(value1);
      else
        js->errorMsg("Invalid incr/decr for this");

      if (postOp_)
        return data.value;
      else
         return value1;
    }
    else
      return data.value;
  }
  else if (iexpr_) {
    if (assign_) {
      CJValueP avalue = assign_->exec(js);

      iexpr_->setIndexValue(js, dict, avalue);

      return avalue;
    }
    else {
      return iexpr_->indexValue(js, dict);
    }
  }
  else
    return dict;
}

void
CJExecThis::
print(std::ostream &os) const
{
  if (incrDecr_ && ! postOp_)
    os << "++";

  os << "this";

  if      (identifiers_)
    os << "." << *identifiers_;
  else if (iexpr_)
    os <<  *iexpr_;

  if (assign_)
    os << " = " << *assign_;

  if (incrDecr_ && postOp_)
    os << "++";

  if (assign_ || incrDecr_)
    os << ";";
}

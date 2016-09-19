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

    CJPropertyData data(js);

    if (assign_ || incrDecr_)
      data.setCreate(true);

    data.setModifiable(true);

    if (! js->lookupScopePropertyData(dict, identifiers, data))
      return CJValueP();

    if      (assign_) {
      CJValueP avalue = assign_->exec(js);

      CJLValueP lvalue = data.lvalue();

      if (! lvalue) {
        js->errorMsg("Invalid assign for this");
        return CJValueP();
      }

      lvalue->setValue(avalue);

      return avalue;
    }
    else if (incrDecr_) {
      CJValueP dvalue = data.value();

      if (dvalue->type() == CJValue::Type::Null)
        dvalue = js->createNumberValue(0L);

      CJValueP value1 = js->execUnaryOp(incrDecr_->type(), dvalue);

      CJLValueP dlvalue = data.lvalue();

      if (! dlvalue) {
        js->errorMsg("Invalid incr/decr for this");
        return CJValueP();
      }

      dlvalue->setValue(value1);

      if (postOp_)
        return dvalue;
      else
        return value1;
    }
    else {
      CJValueP dvalue = data.value();

      return dvalue;
    }
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

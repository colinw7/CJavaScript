#include <CJExecThis.h>
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
  CJDictionaryP dict = js->currentScope();

  if      (identifiers_) {
    const CJavaScript::Identifiers &identifiers = identifiers_->identifiers();

    CJPropertyData data;

    if (! js->lookupPropertyData(dict, identifiers, data))
      return CJValueP();

    if (assign_) {
      CJValueP avalue = assign_->exec(js);

      if (data.lvalue)
        data.lvalue->setValue(avalue);
      else
        js->errorMsg("Invalid assign for this");

      return avalue;
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
  os << "this";

  if      (identifiers_)
    os << "." << *identifiers_;
  else if (iexpr_)
    os <<  *iexpr_;

  if (assign_)
    os << " = " << *assign_;
}

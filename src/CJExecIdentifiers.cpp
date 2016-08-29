#include <CJExecIdentifiers.h>
#include <CJavaScript.h>

CJExecIdentifiers::
CJExecIdentifiers() :
 CJToken(CJToken::Type::Identifiers)
{
}

CJValueP
CJExecIdentifiers::
exec(CJavaScript *js)
{
  if (evalue_) {
    if (identifiers_.empty())
      return CJValueP();

    const std::string &name1 = identifiers_[0]->name();

    if      (evalue_->type() == CJToken::Type::Dictionary) {
      CJDictionaryP dict = std::static_pointer_cast<CJDictionary>(evalue_);

      return dict->getProperty(js, name1);
    }
    else if (evalue_->type() == CJToken::Type::Function) {
      CJFunctionP fn = std::static_pointer_cast<CJFunction>(evalue_);

      return fn->getProperty(js, name1);
    }
    else {
      CJObjTypeP valueType = evalue_->valueType();

      if (evalue_->type() == CJToken::Type::Object) {
        CJValueP propVal = evalue_->cast<CJObj>()->getProperty(js, name1);

        if (propVal)
          return propVal;
      }

      if (valueType->hasProperty(js, name1)) {
        CJValueP propVal = valueType->getProperty(js, name1);

        //CJObjType::Values values;

        //values.push_back(evalue_);

        //return valueType->exec(js, name1, values);
        return propVal;
      }

      return CJValueP();
    }
  }
  else {
    CJValueP value;

    if (isThis_) {
      CJDictionaryP scope = js->thisScope();

      if (! identifiers_.empty()) {
        CJPropertyData data;

        if (! js->lookupPropertyData(scope, identifiers_, data))
          return CJValueP();

        return data.value;
      }
      else
        return scope;
    }
    else if (! identifiers_.empty()) {
      value = js->lookupValue(identifiers_);
    }

    return value;
  }
}

void
CJExecIdentifiers::
print(std::ostream &os) const
{
  int i = 0;

  if (isThis_) {
    os << "this";

    ++i;
  }

  for (auto &id : identifiers_) {
    if (i > 0)
      os << ".";

    os << id->name();

    ++i;
  }
}

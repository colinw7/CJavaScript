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

    CJValueP evalue = evalue_;

    for (uint i = 0; i < identifiers_.size(); ++i) {
      const std::string &name1 = identifiers_[i]->name();

      CJObjTypeP valueType = evalue->valueType();

      if (evalue->hasProperty()) {
        if (evalue->hasPropertyValue(name1)) {
          evalue = evalue->propertyValue(name1);
          continue;
        }
      }

      if (evalue->type() == CJToken::Type::Object) {
        CJValueP propVal = evalue->cast<CJObj>()->getProperty(js, name1);

        if (propVal) {
          evalue = propVal;
          continue;
        }
      }

      if (valueType->hasProperty(js, name1)) {
        CJValueP propVal = valueType->getProperty(js, name1);

        //CJObjType::Values values;

        //values.push_back(evalue);

        //return valueType->exec(js, name1, values);
        evalue = propVal;

        continue;
      }

      js->errorMsg(this, "Invalid value for idenitifer");

      return CJValueP();
    }

    return evalue;
  }
  else {
    CJValueP value;

    if      (isThis_) {
      CJDictionaryP scope = js->thisScope();

      if (! identifiers_.empty()) {
        CJPropertyData data(js);

        if (js->lookupScopePropertyData(scope, identifiers_, data))
          value = data.value();
      }
      else
        return scope;
    }
    else if (! identifiers_.empty()) {
      value = js->lookupValue(identifiers_);
    }

    //if (! value)
    //  value = js->createUndefinedValue();

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

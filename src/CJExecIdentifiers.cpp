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
        if (evalue->hasPropertyValue(name1, /*inherited*/true)) {
          evalue = evalue->propertyValue(name1);
          continue;
        }
      }

      if (evalue->type() == CJToken::Type::Object) {
        CJValueP propVal = CJValue::cast<CJObj>(evalue)->getProperty(js, name1);

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

      js->errorMsg(this, "Invalid value for identifier");

      return CJValueP();
    }

    return evalue;
  }
  else {
    CJExecIdentifiersP eidentifiers = shared_from_this();

    CJValueP value;

    CJPropertyData data(js);

    if (js->lookupPropertyData(eidentifiers, data))
      value = data.value();

    //if (! value)
    //  value = js->createUndefinedValue();

    return value;
  }
}

std::string
CJExecIdentifiers::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJExecIdentifiers::
print(std::ostream &os) const
{
  int i = 0;

  if (isThis()) {
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

#include <CJGetterSetter.h>
#include <CJFunction.h>
#include <CJavaScript.h>

CJObjTypeP CJGetterSetterType::type_;

CJObjTypeP
CJGetterSetterType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJGetterSetterType(js));

    js->addObjectType("GetterSetter", type_);
  }

  return type_;
}

CJGetterSetterType::
CJGetterSetterType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::GetterSetter, "GetterSetter")
{
}

CJValueP
CJGetterSetterType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJGetterSetter::
CJGetterSetter(CJavaScript *js, CJFunctionP getter, CJFunctionP setter) :
 CJValue(CJGetterSetterType::instance(js)), js_(js), getter_(getter), setter_(setter)
{
}

CJGetterSetter::
CJGetterSetter(const CJGetterSetter &gs) :
 CJValue(gs), CJNameSpace(gs), js_(gs.js_), getter_(gs.getter_), setter_(gs.setter_),
 value_(gs.value_)
{
}

CJValueP
CJGetterSetter::
getValue() const
{
  if (! getter_) {
    if (js_->isStrict())
      js_->throwTypeError(nullptr, "No Getter for property");
    return CJValueP();
  }

  CJFunctionBase::Values values;

  CJValueP value = getter_->exec(js_, values);

  return value;
}

void
CJGetterSetter::
setValue(CJValueP value)
{
  if (! setter_) {
    if (js_->isStrict())
      js_->throwTypeError(nullptr, "No Setter for property");
    return;
  }

  CJFunctionBase::Values values;

  values.push_back(value);

  setter_->exec(js_, values);
}

void
CJGetterSetter::
print(std::ostream &os) const
{
  if      (getter_ && setter_)
    os << "get: " << *getter_ << ", set: " << *setter_;
  else if (getter_)
    os << "get: " << *getter_;
  else if (setter_)
    os << "set: " << *setter_;
}

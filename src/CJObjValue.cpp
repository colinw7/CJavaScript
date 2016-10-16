#include <CJObjValue.h>
#include <CJFunction.h>
#include <CJavaScript.h>

CJObjTypeP CJObjValueType::type_;

CJObjTypeP
CJObjValueType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJObjValueType(js));

    js->addObjectType("ObjValue", type_);
  }

  return type_;
}

CJObjValueType::
CJObjValueType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::ObjValue, "ObjValue")
{
}

CJValueP
CJObjValueType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJObjValue::
CJObjValue(CJavaScript *js, CJObjP obj, const std::string &name) :
 CJValue(CJObjValueType::instance(js)), js_(js), obj_(obj), name_(name)
{
}

CJValueP
CJObjValue::
getValue() const
{
  return obj_->getProperty(js_, name_);
}

void
CJObjValue::
setValue(CJValueP value)
{
  obj_->setProperty(js_, name_, value);
}

bool
CJObjValue::
toBoolean() const
{
  return getValue()->toBoolean();
}

COptReal
CJObjValue::
toReal() const
{
  return getValue()->toReal();
}

COptLong
CJObjValue::
toInteger() const
{
  return getValue()->toInteger();
}

std::string
CJObjValue::
toString() const
{
  return getValue()->toString();
}

void
CJObjValue::
print(std::ostream &os) const
{
  getValue()->print(os);
}

#include <CJTypeFunction.h>
#include <CJavaScript.h>
#include <CJString.h>
#include <CJArray.h>

CJTypeFunction::
CJTypeFunction(CJavaScript *js, const std::string &name, CJObjTypeP type) :
 CJFunctionBase(js, name, CJFunctionBase::Type::Type), type_(type)
{
}

CJTypeFunction::
CJTypeFunction(const CJTypeFunction &fn) :
 CJFunctionBase(fn), type_(fn.type_)
{
}

CJValueP
CJTypeFunction::
getProperty(CJavaScript *js, const std::string &name) const
{
  if (type_->hasProperty(js, name))
    return type_->getProperty(js, name);

  return CJFunctionBase::getProperty(js, name);
}

void
CJTypeFunction::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  if (type_ && type_->hasProperty(js, name))
    type_->setProperty(js, name, value);
  else
    CJFunctionBase::setProperty(js, name, value);
}

// call type function for type
CJValueP
CJTypeFunction::
exec(CJavaScript *js, const Values &values)
{
  assert(values.size() > 0);

  CJValueP value;

  if (! values[0])
    return value;

  CJValueP ovalue = values[0];

  if      (ovalue->type() == CJToken::Type::Object) {
    value = CJValue::cast<CJObj>(ovalue)->execNameFn(js, name(), values);
  }
  else if (ovalue->type() == CJToken::Type::String) {
    CJObjTypeP stringType = CJStringType::instance(js);

    value = stringType->exec(js, name(), values);
  }
  else if (ovalue->type() == CJToken::Type::Array) {
    CJObjTypeP arrayType = CJArrayType::instance(js);

    value = arrayType->exec(js, name(), values);
  }
  else if (ovalue->type() == CJToken::Type::Date) {
    CJObjTypeP dateType = CJDateType::instance(js);

    value = dateType->exec(js, name(), values);
  }
  else if (ovalue->isFunction()) {
    CJTypeFunctionP typeFn = CJValue::cast<CJTypeFunction>(ovalue);

    CJObjTypeP objType = typeFn->objectType();

    value = objType->exec(js, name(), values);
  }
  else if (ovalue->type() == CJToken::Type::Dictionary) {
    js->errorMsg("Invalid dictionary type function");
  }
  else {
    js->errorMsg("Invalid object function type");
  }

  return value;
}

std::string
CJTypeFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJTypeFunction::
print(std::ostream &os) const
{
  os << "[Function: " + name_ + "]";
}

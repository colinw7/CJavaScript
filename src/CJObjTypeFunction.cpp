#include <CJObjTypeFunction.h>
#include <CJavaScript.h>
#include <CJString.h>
#include <CJArray.h>

CJObjTypeFunction::
CJObjTypeFunction(CJavaScript *js, const std::string &name, CJObjTypeP type) :
 CJFunctionBase(js, name, CJFunctionBase::Type::ObjType), type_(type)
{
}

CJValueP
CJObjTypeFunction::
getProperty(CJavaScript *js, const std::string &name) const
{
  if (type_ && type_->hasProperty(js, name))
    return type_->getProperty(js, name);

  return CJFunctionBase::getProperty(js, name);
}

void
CJObjTypeFunction::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  if (type_ && type_->hasProperty(js, name))
    type_->setProperty(js, name, value);
  else
    CJFunctionBase::setProperty(js, name, value);
}

// call type function for type
CJValueP
CJObjTypeFunction::
exec(CJavaScript *js, const Values &values)
{
  assert(values.size() > 0);

  CJValueP value;

  if (! values[0])
    return value;

  CJValueP ovalue = values[0];

  CJObjTypeP objType = type_;

  if (! objType) {
    if (ovalue->isFunction()) {
      CJObjTypeFunctionP typeFn = CJValue::cast<CJObjTypeFunction>(ovalue);

      objType = typeFn->objectType();
    }
    else {
      objType = js->getTypeObject(ovalue->type());
    }
  }

  if (objType)
    value = objType->execType(js, name(), values);
  else
    js->errorMsg(this, "Invalid object function type");

  return value;
}

CJValueP
CJObjTypeFunction::
execNew(CJavaScript *, const Values &)
{
  assert(false);
}

std::string
CJObjTypeFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJObjTypeFunction::
print(std::ostream &os) const
{
  os << "[Function: " + name_ + "]";
}

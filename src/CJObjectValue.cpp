#include <CJObjectValue.h>
#include <CJObj.h>

CJObjectValue::
CJObjectValue(CJavaScript *js, CJObjP obj, const std::string &name) :
 CJLValue(obj->objType()), js_(js), obj_(obj), name_(name)
{
}

CJObjectValue::
CJObjectValue(const CJObjectValue &value) :
 CJLValue(value), js_(value.js_), obj_(value.obj_), name_(value.name_)
{
}

CJValueP
CJObjectValue::
value() const
{
  CJValueP val = obj_->getProperty(js_, name_);

  if (! val)
    val = obj_->valueType()->getProperty(js_, name_);

  return val;
}

void
CJObjectValue::
setValue(CJValueP value)
{
  obj_->setPropertyValue(name_, value);
}

void
CJObjectValue::
print(std::ostream &os) const
{
  os << *obj_ << " :" << name_;
}

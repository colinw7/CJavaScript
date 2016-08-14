#include <CJObjectValue.h>
#include <CJObj.h>
#include <CJObject.h>

CJObjectValue::
CJObjectValue(CJavaScript *js, CJObjP obj, const std::string &name) :
 CJLValue(obj->type()), js_(js), obj_(obj), name_(name)
{
}

CJValueP
CJObjectValue::
value() const
{
  CJValueP val = obj_->getProperty(name_);

  if (! val)
    val = obj_->valueType()->getProperty(name_);

  return val;
}

void
CJObjectValue::
setValue(CJValueP value)
{
  obj_->setProperty(name_, value);
}

void
CJObjectValue::
print(std::ostream &os) const
{
  os << *obj_ << " :" << name_;
}

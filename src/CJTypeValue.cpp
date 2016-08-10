#include <CJTypeValue.h>
#include <CJObjectType.h>

CJTypeValue::
CJTypeValue(CJavaScript *js, CJObjectTypeP objType, const std::string &name) :
 CJLValue(objType), js_(js), objType_(objType), name_(name)
{
}

bool
CJTypeValue::
hasValue() const
{
  return objType_->hasProperty(name_);
}

CJValueP
CJTypeValue::
value() const
{
  return objType_->getProperty(name_);
}

void
CJTypeValue::
setValue(CJValueP value)
{
  objType_->setProperty(name_, value);
}

void
CJTypeValue::
print(std::ostream &os) const
{
  os << *objType_ << " :" << name_;
}

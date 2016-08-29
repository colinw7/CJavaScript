#include <CJTypeValue.h>
#include <CJObj.h>

CJTypeValue::
CJTypeValue(CJavaScript *js, CJObjTypeP objType, const std::string &name) :
 CJLValue(objType), js_(js), objType_(objType), name_(name)
{
}

bool
CJTypeValue::
hasValue() const
{
  return objType_->hasProperty(js_, name_);
}

CJValueP
CJTypeValue::
value() const
{
  return objType_->getProperty(js_, name_);
}

void
CJTypeValue::
setValue(CJValueP value)
{
  objType_->setProperty(js_, name_, value);
}

void
CJTypeValue::
print(std::ostream &os) const
{
  os << *objType_ << " :" << name_;
}

#include <CJPropertyData.h>
#include <CJNameSpaceValue.h>
#include <CJObjectValue.h>
#include <CJavaScript.h>

CJValueP
CJPropertyData::
value() const
{
  if (! value_ && create_) {
    value_ = js_->createNullValue();

    if      (scope_ && name_ != "")
      scope_->setProperty(js_, name_, value_);
    else if (obj_ && name_ != "")
      obj_->setProperty(js_, name_, value_);
  }

  // get value from object
  if (! value_ && modifiable_) {
    if      (scope_ && name_ != "")
      value_ = scope_->getProperty(js_, name_);
    else if (obj_ && name_ != "")
      value_ = obj_->getProperty(js_, name_);
  }

  if (value_)
    return value_;

  // get lvalue value
  CJLValueP lvalue = this->lvalue();

  return (lvalue ? lvalue->value() : CJValueP());
}

CJLValueP
CJPropertyData::
lvalue() const
{
  // get namespace of object lvalue
  if (! lvalue_ && modifiable_) {
    if      (scope_ && name_ != "")
      lvalue_ = CJLValueP(new CJNameSpaceValue(js_, scope_, name_));
    else if (obj_ && name_ != "")
      lvalue_ = CJLValueP(new CJObjectValue(js_, obj_, name_));
  }

  return lvalue_;
}

#include <CJValue.h>
#include <CJObj.h>
#include <CJUtil.h>

CJValue::
CJValue(CJObjTypeP valueType) :
 CJToken(valueType->type()), valueType_(valueType)
{
}

CJValue::
CJValue(const CJValue &v) :
 CJToken(v), std::enable_shared_from_this<CJValue>(v), valueType_(v.valueType_)
{
}

OptLong
CJValue::
toInteger() const
{
  OptReal optReal = toReal();

  if (! optReal)
    return OptLong();

  double r = optReal.value();

  if (CJUtil::isNaN(r))
    return OptLong(0);

  if (CJUtil::isPosInf(r))
    return OptLong(CJUtil::maxInteger());

  if (CJUtil::isNegInf(r))
    return OptLong(CJUtil::minInteger());

  return OptLong(long(r));
}

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

COptLong
CJValue::
toInteger() const
{
  COptReal optReal = toReal();

  if (! optReal.isValid())
    return COptLong();

  double r = optReal.getValue();

  if (CJUtil::isNaN(r))
    return COptLong(0);

  if (CJUtil::isPosInf(r))
    return COptLong(CJUtil::maxInteger());

  if (CJUtil::isNegInf(r))
    return COptLong(CJUtil::minInteger());

  return COptLong(long(r));
}

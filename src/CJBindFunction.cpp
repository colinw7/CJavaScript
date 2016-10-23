#include <CJBindFunction.h>
#include <CJavaScript.h>

CJBindFunction::
CJBindFunction(CJavaScript *js, CJFunctionBaseP function,
               CJValueP thisValue, const Values &values) :
 CJFunctionBase(js, "bind", CJFunctionBase::Type::Bind),
 function_(function), thisValue_(thisValue), values_(values)
{
}

CJValueP
CJBindFunction::
exec(CJavaScript *js, const Values &values)
{
  Values callValues;

  callValues.push_back(thisValue_);

  for (uint i = 1; i < values.size(); ++i)
    callValues.push_back(values[i]);

  for (uint i = 1; i < values_.size(); ++i)
    callValues.push_back(values_[i]);

  CJValueP res;

  if (thisValue_->isObject()) {
    js->pushThis(CJValue::cast<CJObj>(thisValue_));

    res = function_->exec(js, callValues);

    js->popThis();
  }
  else
    res = function_->exec(js, callValues);

  return res;
}

std::string
CJBindFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJBindFunction::
print(std::ostream &os) const
{
  os << "bind " << function_->name() << "(" << *thisValue_;

  for (uint i = 1; i < values_.size(); ++i)
    os << ", " << *values_[i];

  os << ")";
}

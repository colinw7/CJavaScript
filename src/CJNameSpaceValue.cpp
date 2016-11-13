#include <CJNameSpaceValue.h>
#include <CJDictionary.h>

CJNameSpaceValue::
CJNameSpaceValue(CJavaScript *js, CJNameSpaceP scope, const std::string &name) :
 CJLValue(CJDictionaryType::instance(js)), js_(js), scope_(scope), name_(name)
{
}

CJNameSpaceValue::
CJNameSpaceValue(const CJNameSpaceValue &value) :
 CJLValue(value), js_(value.js_), scope_(value.scope_), name_(value.name_)
{
}

std::string
CJNameSpaceValue::
toString() const
{
  CJValueP v = value();

  return (v ? v->toString() : "");
}

COptReal
CJNameSpaceValue::
toReal() const
{
  CJValueP v = value();

  return (v ? v->toReal() : COptReal());
}

COptLong
CJNameSpaceValue::
toInteger() const
{
  CJValueP v = value();

  return (v ? v->toInteger() : COptLong());
}

bool
CJNameSpaceValue::
toBoolean() const
{
  CJValueP v = value();

  return (v ? v->toBoolean() : false);
}

CJValueP
CJNameSpaceValue::
value() const
{
  return scope_->getProperty(js_, name_);
}

void
CJNameSpaceValue::
setValue(CJValueP value)
{
  if (scope_->isWritableProperty(name_))
    scope_->setProperty(js_, name_, value);
}

bool
CJNameSpaceValue::
isWritable() const
{
  return scope_->isWritableProperty(name_);
}

void
CJNameSpaceValue::
setWritable(bool b)
{
  scope_->setWritableProperty(name_, b);
}

void
CJNameSpaceValue::
print(std::ostream &os) const
{
  CJValueP v = value();

  if (v)
    v->print(os);
}

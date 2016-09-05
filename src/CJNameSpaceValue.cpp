#include <CJNameSpaceValue.h>
#include <CJDictionary.h>

CJNameSpaceValue::
CJNameSpaceValue(CJavaScript *js, CJNameSpaceP scope, const std::string &name) :
 CJLValue(CJDictionaryType::instance(js)), js_(js), scope_(scope), name_(name)
{
}

std::string
CJNameSpaceValue::
toString() const
{
  CJValueP v = value();

  return (v ? v->toString() : "");
}

double
CJNameSpaceValue::
toReal() const
{
  CJValueP v = value();

  return (v ? v->toReal() : 0.0);
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
  if (! scope_->isReadOnlyProperty(name_))
    scope_->setProperty(js_, name_, value);
}

bool
CJNameSpaceValue::
isReadOnly() const
{
  return scope_->isReadOnlyProperty(name_);
}

void
CJNameSpaceValue::
setReadOnly(bool b)
{
  scope_->setReadOnlyProperty(name_, b);
}

void
CJNameSpaceValue::
print(std::ostream &os) const
{
  CJValueP v = value();

  if (v)
    v->print(os);
}
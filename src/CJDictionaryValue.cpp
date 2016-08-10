#include <CJDictionaryValue.h>
#include <CJDictionary.h>

CJDictionaryValue::
CJDictionaryValue(CJavaScript *js, CJDictionaryP dict, const std::string &name) :
 CJLValue(CJDictionaryType::instance(js)), js_(js), dict_(dict), name_(name)
{
}

std::string
CJDictionaryValue::
toString() const
{
  CJValueP v = value();

  return (v ? v->toString() : "");
}

double
CJDictionaryValue::
toReal() const
{
  CJValueP v = value();

  return (v ? v->toReal() : 0.0);
}

bool
CJDictionaryValue::
toBoolean() const
{
  CJValueP v = value();

  return (v ? v->toBoolean() : false);
}

CJValueP
CJDictionaryValue::
value() const
{
  return dict_->getProperty(name_);
}

void
CJDictionaryValue::
setValue(CJValueP value)
{
  return dict_->setProperty(name_, value);
}

void
CJDictionaryValue::
print(std::ostream &os) const
{
  CJValueP v = value();

  if (v)
    v->print(os);
}

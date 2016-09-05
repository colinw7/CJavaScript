#include <CJDictionaryRef.h>
#include <CJDictionary.h>

CJDictionaryRef::
CJDictionaryRef(CJavaScript *js, CJDictionary *dict, const std::string &name) :
 CJLValue(CJDictionaryType::instance(js)), js_(js), dict_(dict), name_(name)
{
}

CJValueP
CJDictionaryRef::
value() const
{
  return dict_->getProperty(js_, name_);
}

void
CJDictionaryRef::
setValue(CJValueP value)
{
  dict_->setProperty(js_, name_, value);
}

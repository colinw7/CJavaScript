#include <CJNameSpace.h>
#include <CJavaScript.h>
#include <CJFunction.h>

void
CJNameSpace::
setRealProperty(CJavaScript *js, const std::string &key, double r)
{
  CJValueP value = js->createNumberValue(r);

  setProperty(key, value);
}

void
CJNameSpace::
setIntegerProperty(CJavaScript *js, const std::string &key, long i)
{
  CJValueP value = js->createNumberValue(i);

  setProperty(key, value);
}

void
CJNameSpace::
setStringProperty(CJavaScript *js, const std::string &key, const std::string &str)
{
  CJValueP value = js->createStringValue(str);

  setProperty(key, value);
}

void
CJNameSpace::
setFunctionProperty(CJavaScript *, const std::string &key, CJFunctionP function)
{
  CJValueP value = std::static_pointer_cast<CJValue>(function);

  setProperty(key, value);
}

void
CJNameSpace::
setFunctionProperty(CJavaScript *, CJFunctionP function)
{
  CJValueP value = std::static_pointer_cast<CJValue>(function);

  setProperty(function->name(), value);
}

void
CJNameSpace::
setProperty(const std::string &key, CJValueP value)
{
  keyValues_[key] = value;
}

bool
CJNameSpace::
hasProperty(const std::string &key) const
{
  if (keyValues_.find(key) != keyValues_.end())
    return true;

  if (keyNames_.find(key) != keyNames_.end())
    return true;

  return false;
}

CJValueP
CJNameSpace::
getProperty(const std::string &key) const
{
  auto p = keyValues_.find(key);

  if (p != keyValues_.end())
    return (*p).second;

  return CJValueP();
}

std::string
CJNameSpace::
getStringProperty(const std::string &key, const std::string &def) const
{
  CJValueP v = getProperty(key);
  if (! v) return def;

  return v->toString();
}

double
CJNameSpace::
getRealProperty(const std::string &key, double def) const
{
  CJValueP v = getProperty(key);
  if (! v) return def;

  return v->toReal();
}

CJNameSpace::Names
CJNameSpace::
getPropertyNames(bool pseudo) const
{
  Names names;

  for (const auto &kv : keyValues_)
    names.push_back(kv.first);

  if (pseudo) {
    for (const auto &n : keyNames_)
      names.push_back(n);
  }

  return names;
}

void
CJNameSpace::
deleteProperty(const std::string &key)
{
  keyValues_.erase(key);
}

void
CJNameSpace::
addPseudoProperty(const std::string &key)
{
  keyNames_.insert(key);
}

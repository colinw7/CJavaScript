#include <CJNameSpace.h>
#include <CJavaScript.h>
#include <CJFunction.h>

void
CJNameSpace::
setRealProperty(CJavaScript *js, const std::string &key, double r)
{
  CJValueP value = js->createNumberValue(r);

  setProperty(js, key, value);
}

void
CJNameSpace::
setIntegerProperty(CJavaScript *js, const std::string &key, long i)
{
  CJValueP value = js->createNumberValue(i);

  setProperty(js, key, value);
}

void
CJNameSpace::
setStringProperty(CJavaScript *js, const std::string &key, const std::string &str)
{
  CJValueP value = js->createStringValue(str);

  setProperty(js, key, value);
}

void
CJNameSpace::
setFunctionProperty(CJavaScript *js, const std::string &key, CJFunctionP function)
{
  CJValueP value = std::static_pointer_cast<CJValue>(function);

  setProperty(js, key, value);
}

void
CJNameSpace::
setFunctionProperty(CJavaScript *js, CJFunctionP function)
{
  CJValueP value = std::static_pointer_cast<CJValue>(function);

  setProperty(js, function->name(), value);
}

void
CJNameSpace::
setProperty(CJavaScript *, const std::string &key, CJValueP value)
{
  keyValues_[key] = value;
}

bool
CJNameSpace::
hasProperty(CJavaScript *, const std::string &key) const
{
  if (keyValues_.find(key) != keyValues_.end())
    return true;

  if (keyNames_.find(key) != keyNames_.end())
    return true;

  return false;
}

CJValueP
CJNameSpace::
getProperty(CJavaScript *, const std::string &key) const
{
  auto p = keyValues_.find(key);

  if (p != keyValues_.end())
    return (*p).second;

  return CJValueP();
}

std::string
CJNameSpace::
getStringProperty(CJavaScript *js, const std::string &key, const std::string &def) const
{
  CJValueP v = getProperty(js, key);
  if (! v) return def;

  return v->toString();
}

double
CJNameSpace::
getRealProperty(CJavaScript *js, const std::string &key, double def) const
{
  CJValueP v = getProperty(js, key);
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

bool
CJNameSpace::
isReadOnlyProperty(const std::string &key)  const
{
  auto p = readOnly_.find(key);

  if (p != readOnly_.end())
    return (*p).second;

  return false;
}

void
CJNameSpace::
setReadOnlyProperty(const std::string &key, bool b)
{
  readOnly_[key] = b;
}

void
CJNameSpace::
print(std::ostream &os) const
{
  int i = 0;

  for (const auto &kv : keyValues_) {
    if (i > 0) os << " ";

    os << kv.first << ": " << *kv.second;

    ++i;
  }

  for (const auto &n : keyNames_) {
    if (i > 0) os << " ";

    os << n << ": <pseudo>";

    ++i;
  }
}

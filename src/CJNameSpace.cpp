#include <CJNameSpace.h>
#include <CJavaScript.h>

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
setBoolProperty(CJavaScript *js, const std::string &key, bool b)
{
  CJValueP value = js->createBoolValue(b);

  setProperty(js, key, value);
}

void
CJNameSpace::
setFunctionProperty(CJavaScript *js, const std::string &key, CJFunctionBaseP function)
{
  CJValueP value = function;

  setProperty(js, key, value);

  CJFunctionBase::addFunctionMethods(js, function, CJObjTypeP());
}

void
CJNameSpace::
setFunctionProperty(CJavaScript *js, CJFunctionBaseP function, CJObjTypeP objType)
{
  CJValueP value = function;

  setProperty(js, function->name(), value);

  CJFunctionBase::addFunctionMethods(js, function, objType);
}

bool
CJNameSpace::
getPropertyData(CJavaScript *, const std::string &key, CJPropertyValue &data)
{
  auto p = keyValues_.find(key);

  if (p == keyValues_.end())
    return false;

  data = (*p).second;

  return true;
}

void
CJNameSpace::
setPropertyData(CJavaScript *js, const std::string &key, const CJPropertyValue &data)
{
  auto p = keyValues_.find(key);

  if (p == keyValues_.end())
    p = keyValues_.insert(p, KeyValues::value_type(key, CJPropertyValue()));

  (*p).second = data;

  setProperty(js, key, data.value);
}

void
CJNameSpace::
setProperty(CJavaScript *, const std::string &key, CJValueP value)
{
  auto p = keyValues_.find(key);

  if (p == keyValues_.end())
    p = keyValues_.insert(p, KeyValues::value_type(key, CJPropertyValue()));

  (*p).second.value = value;
}

bool
CJNameSpace::
hasProperty(CJavaScript *, const std::string &key, bool /*inherit*/) const
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

  if (p == keyValues_.end())
    return CJValueP();

  CJValueP value = (*p).second.value;

  return value;
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

  return v->toReal().getValue(0.0);
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

bool
CJNameSpace::
deletePropertyIndices(CJavaScript *js, const std::string &key, const Values &ivalues)
{
  if (ivalues.empty()) {
    if (keyValues_.find(key) == keyValues_.end())
      return false;

    deleteProperty(js, key);
  }
  else {
    auto p = keyValues_.find(key);

    if (p == keyValues_.end())
      return false;

    CJValueP value = (*p).second.value;

    if (! value)
      return false;

    uint i = 0;

    while (i < ivalues.size() - 1) {
      CJValueP rvalue;

      if (! js->indexValue(value, ivalues[i], rvalue))
        return false;

      value = rvalue;

      ++i;
    }

    if (! js->deleteIndexValue(value, ivalues.back()))
      return false;
  }

  return true;
}

void
CJNameSpace::
deleteProperty(CJavaScript *, const std::string &key)
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
isWritableProperty(const std::string &key) const
{
  auto p = keyValues_.find(key);

  if (p == keyValues_.end())
    return true;

  return (*p).second.writable.getValue(true);
}

void
CJNameSpace::
setWritableProperty(const std::string &key, bool b)
{
  auto p = keyValues_.find(key);

  if (p == keyValues_.end())
    p = keyValues_.insert(p, KeyValues::value_type(key, CJPropertyValue()));

  (*p).second.writable = b;
}

bool
CJNameSpace::
isEnumerableProperty(const std::string &key) const
{
  auto p = keyValues_.find(key);

  if (p == keyValues_.end())
    return true;

  return (*p).second.enumerable.getValue(true);
}

void
CJNameSpace::
setEnumerableProperty(const std::string &key, bool b)
{
  auto p = keyValues_.find(key);

  if (p == keyValues_.end())
    p = keyValues_.insert(p, KeyValues::value_type(key, CJPropertyValue()));

  (*p).second.enumerable = b;
}

bool
CJNameSpace::
isConfigurableProperty(const std::string &key) const
{
  auto p = keyValues_.find(key);

  if (p == keyValues_.end())
    return true;

  return (*p).second.configurable.getValue(true);
}

void
CJNameSpace::
setConfigurableProperty(const std::string &key, bool b)
{
  auto p = keyValues_.find(key);

  if (p == keyValues_.end())
    p = keyValues_.insert(p, KeyValues::value_type(key, CJPropertyValue()));

  (*p).second.configurable = b;
}

std::string
CJNameSpace::
toString() const
{
  std::ostringstream ss; ss << *this;
  return ss.str();
}

void
CJNameSpace::
print(std::ostream &os) const
{
  int i = 0;

  for (const auto &kv : keyValues_) {
    if (i > 0) os << " ";

    os << kv.first << ": " << *kv.second.value;

    ++i;
  }

  for (const auto &n : keyNames_) {
    if (i > 0) os << " ";

    os << n << ": <pseudo>";

    ++i;
  }
}

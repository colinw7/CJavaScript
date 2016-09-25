#include <CJNameSpace.h>
#include <CJavaScript.h>
#include <CJCallFunction.h>

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
setFunctionProperty(CJavaScript *js, const std::string &key, CJFunctionBaseP function)
{
  CJValueP value = std::static_pointer_cast<CJValue>(function);

  setProperty(js, key, value);

  function->setProperty(js, "call", CJValueP(new CJCallFunction(js, function)));
}

void
CJNameSpace::
setFunctionProperty(CJavaScript *js, CJFunctionBaseP function)
{
  CJValueP value = std::static_pointer_cast<CJValue>(function);

  setProperty(js, function->name(), value);

  function->setProperty(js, "call", CJValueP(new CJCallFunction(js, function)));
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

  if (p == keyValues_.end())
    return CJValueP();

  CJValueP value = (*p).second;

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

bool
CJNameSpace::
deleteProperty(CJavaScript *js, const std::string &key, const Values &ivalues)
{
  if (ivalues.empty()) {
    if (keyValues_.find(key) == keyValues_.end())
      return false;

    deleteProperty(key);
  }
  else {
    auto p = keyValues_.find(key);

    if (p == keyValues_.end())
      return false;

    CJValueP value = (*p).second;

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
canDeleteProperty(const std::string &key) const
{
  auto p = propertyMap_.find(key);

  if (p != propertyMap_.end())
    return (*p).second.canDelete.getValue(true);

  return true;
}

void
CJNameSpace::
setCanDeleteProperty(const std::string &key, bool b)
{
  auto p = propertyMap_.find(key);

  if (p == propertyMap_.end())
    p = propertyMap_.insert(p, PropertyMap::value_type(key, PropertyData()));

  (*p).second.canDelete = b;
}

bool
CJNameSpace::
isWritableProperty(const std::string &key) const
{
  auto p = propertyMap_.find(key);

  if (p != propertyMap_.end())
    return (*p).second.writable.getValue(true);

  return true;
}

void
CJNameSpace::
setWritableProperty(const std::string &key, bool b)
{
  auto p = propertyMap_.find(key);

  if (p == propertyMap_.end())
    p = propertyMap_.insert(p, PropertyMap::value_type(key, PropertyData()));

  (*p).second.writable = b;
}

bool
CJNameSpace::
isEnumerableProperty(const std::string &key) const
{
  auto p = propertyMap_.find(key);

  if (p != propertyMap_.end())
    return (*p).second.enumerable.getValue(true);

  return true;
}

void
CJNameSpace::
setEnumerableProperty(const std::string &key, bool b)
{
  auto p = propertyMap_.find(key);

  if (p == propertyMap_.end())
    p = propertyMap_.insert(p, PropertyMap::value_type(key, PropertyData()));

  (*p).second.enumerable = b;
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

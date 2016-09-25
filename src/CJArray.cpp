#include <CJArray.h>
#include <CJavaScript.h>
#include <algorithm>

CJObjTypeP CJArrayType::type_;

CJObjTypeP
CJArrayType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJArrayType(js));

    js->addObjectType("array", type_);

    js->addTypeObject(CJToken::Type::Array, type_);
  }

  return type_;
}

CJArrayType::
CJArrayType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Array, "array")
{
  addTypeFunction(js, "from");
  addTypeFunction(js, "isArray");
  addTypeFunction(js, "observe"); // obsolete
  addTypeFunction(js, "of");
  addTypeFunction(js, "unobserve"); // obsolete
  addTypeFunction(js, "toString");

  addObjectFunction(js, "propertyIsEnumerable"); // TODO: move to base class
  addObjectFunction(js, "hasOwnProperty"); // TODO: move to base class

  addObjectFunction(js, "toString");
  addObjectFunction(js, "concat");
  addObjectFunction(js, "join");
  addObjectFunction(js, "pop");
  addObjectFunction(js, "push");
  addObjectFunction(js, "reverse");
  addObjectFunction(js, "shift");
  addObjectFunction(js, "sort");
  addObjectFunction(js, "unshift");
}

CJValueP
CJArrayType::
execType(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  // values[0] is CJArrayFunction

  //---

  // type functions
  if      (name == "from") {
    CJValueP value1;

    if (values.size() > 1)
      value1 = values[1];

    if (! js->isUndefinedOrNullValue(value1)) {
      CJArray *array = new CJArray(js);

      if      (value1->hasIndex()) {
        long len = value1->length();

        for (long ind = 0; ind < len; ++ind)
          array->addValue(value1->indexValue(ind));

        return CJValueP(array);
      }
      else if (value1->hasProperty()) {
        CJValue::KeyNames names = value1->propertyNames();

        for (const auto &ind : names) {
          array->addValue(value1->propertyValue(ind));
        }
      }

      return CJValueP(array);
    }
    else {
      js->throwTypeError(value1, "Cannot convert undefined or null to object");
      return CJValueP();
    }
  }
  else if (name == "isArray") {
    bool b = false;

    if (values.size() > 1)
      b = (values[1]->type() == CJToken::Type::Array);

    return js->createBoolValue(b);
  }
  else if (name == "observe") {
  }
  else if (name == "of") {
    CJArray *array = new CJArray(js);

    for (uint i = 1; i < values.size(); ++i)
      array->addValue(values[i]);

    return CJValueP(array);
  }
  else if (name == "unobserve") {
  }
  else if (name == "toString") {
    return js->createStringValue("function Array() { }");
  }
  else {
    js->errorMsg("Invalid array type function " + name);
  }

  return CJValueP();
}

CJValueP
CJArrayType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJArray *array = values[0]->cast<CJArray>();
  assert(array);

  // object functions
  if      (name == "toString") {
    return js->createStringValue(array->toString());
  }
  else if (name == "concat") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    if      (values[1]->type() == CJToken::Type::String) {
      array->addValue(values[1]);
    }
    else if (values[1]->hasIndex()) {
      int len = values[1]->length();

      for (int i = 0; i < len; ++i) {
        array->addValue(values[1]->indexValue(i));
      }
    }
    else {
      array->addValue(values[1]);
    }

    return values[0];
  }
  else if (name == "join") {
    std::string jstr = ",";

    if (values.size() > 1)
      jstr = values[1]->toString();

    std::string str;

    int len = array->length();

    for (int i = 0; i < len; ++i) {
      if (i > 0)
        str += jstr;

      str += array->indexValue(i)->toString();
    }

    return js->createStringValue(str);
  }
  else if (name == "pop") {
    if (values.size() != 1) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    int len = array->length();

    if (len < 1) {
      js->errorMsg("Empty array for " + name);
      return CJValueP();
    }

    CJValueP value = array->removeValue();

    return value;
  }
  else if (name == "push") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    array->addValue(values[1]);

    return values[0];
  }
  else if (name == "reverse") {
    array->reverse();

    return values[0];
  }
  else if (name == "shift") {
    if (values.size() != 1) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    int len = array->length();

    if (len < 1) {
      js->errorMsg("Empty array for " + name);
      return CJValueP();
    }

    CJValueP value = array->removeFrontValue();

    return value;
  }
  else if (name == "sort") {
    if (values.size() > 1) {
      // TODO: sort function name
    }

    array->sort();

    return values[0];
  }
  else if (name == "unshift") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    array->addFrontValue(values[1]);

    return values[0];
  }
  else if (name == "propertyIsEnumerable") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    int ind = values[1]->toInteger();

    bool b = array->isEnumerableIndex(ind);

    return js->createBoolValue(b);
  }
  else if (name == "hasOwnProperty") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    return js->createBoolValue(js->hasIndexValue(values[0], values[1]));
  }
  else {
    js->errorMsg("Invalid array object function " + name);
  }

  return CJValueP();
}

//------

CJArray::
CJArray(CJavaScript *js, int n) :
 CJObj(js, CJArrayType::instance(js))
{
  if (n > 0)
    values_.resize(n);

  addPseudoProperty("length");
}

CJArray::
CJArray(CJavaScript *js, const Values &values) :
 CJObj(js, CJArrayType::instance(js)), values_(values)
{
  addPseudoProperty("length");
}

CJArray::
CJArray(CJavaScript *js, const std::vector<CJValueP> &values) :
 CJObj(js, CJArrayType::instance(js))
{
  for (const auto &v : values)
    values_.push_back(v);

  addPseudoProperty("length");
}

void
CJArray::
setValues(const std::vector<CJValueP> &values)
{
  values_.clear();

  for (const auto &v : values)
    values_.push_back(v);
}

void
CJArray::
addValue(CJValueP value)
{
  values_.push_back(value);
}

CJValueP
CJArray::
removeValue()
{
  assert(! values_.empty());

  CJValueP value = values_.back();

  values_.pop_back();

  return value;
}

void
CJArray::
addFrontValue(CJValueP value)
{
  values_.push_front(value);
}

CJValueP
CJArray::
removeFrontValue()
{
  assert(! values_.empty());

  CJValueP value = values_.front();

  values_.pop_front();

  return value;
}

bool
CJArray::
hasIndexValue(int ind) const
{
  if (ind < 0 || ind >= int(values_.size()))
    return false;

  return !!values_[ind];
}

CJValueP
CJArray::
indexValue(int ind) const
{
  if (ind < 0 || ind >= int(values_.size()))
    return CJValueP();

  return values_[ind];
}

void
CJArray::
setIndexValue(int ind, CJValueP value)
{
  if (ind < 0)
    return;

  if (! isWritableIndex(ind))
    return;

  while (ind >= int(values_.size()))
    values_.push_back(CJValueP());

  values_[ind] = value;
}

void
CJArray::
deleteIndexValue(int ind)
{
  setIndexValue(ind, CJValueP());
}

bool
CJArray::
canDeleteIndex(int ind) const
{
  auto p = propertyMap_.find(ind);

  if (p != propertyMap_.end())
    return (*p).second.canDelete.getValue(true);

  return true;
}

void
CJArray::
setCanDeleteIndex(int ind, bool b)
{
  auto p = propertyMap_.find(ind);

  if (p == propertyMap_.end())
    p = propertyMap_.insert(p, PropertyMap::value_type(ind, PropertyData()));

  (*p).second.canDelete = b;
}

bool
CJArray::
isWritableIndex(int ind) const
{
  auto p = propertyMap_.find(ind);

  if (p != propertyMap_.end())
    return (*p).second.writable.getValue(true);

  return true;
}

void
CJArray::
setWritableIndex(int ind, bool b)
{
  auto p = propertyMap_.find(ind);

  if (p == propertyMap_.end())
    p = propertyMap_.insert(p, PropertyMap::value_type(ind, PropertyData()));

  (*p).second.writable = b;
}


bool
CJArray::
isEnumerableIndex(int ind) const
{
  auto p = propertyMap_.find(ind);

  if (p != propertyMap_.end())
    return (*p).second.enumerable.getValue(true);

  return true;
}

void
CJArray::
setEnumerableIndex(int ind, bool b)
{
  auto p = propertyMap_.find(ind);

  if (p == propertyMap_.end())
    p = propertyMap_.insert(p, PropertyMap::value_type(ind, PropertyData()));

  (*p).second.enumerable = b;
}

void
CJArray::
reverse()
{
  int len = values_.size();

  for (int i = 0, j = len - 1; i < j; ++i, --j)
    std::swap(values_[i], values_[j]);
}

void
CJArray::
sort()
{
  struct Cmp {
    bool operator()(const CJValueP &v1, const CJValueP &v2) {
      if (! v2) return false;
      if (! v1) return true;
      return (v1->cmp(v2.get()) < 0);
    }
  };

  Cmp cmp;

  std::sort(values_.begin(), values_.end(), cmp);
}

CJValueP
CJArray::
getProperty(CJavaScript *js, const std::string &key) const
{
  if (key == "length")
    return js->createNumberValue(length());

  return CJObj::getProperty(js, key);
}

void
CJArray::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  if (key == "length") {
    uint len = value->toInteger();

    while (values_.size() > len)
      values_.pop_back();
  }
  else
    CJObj::setProperty(js, key, value);
}

bool
CJArray::
hasValue(CJValueP value) const
{
  for (const auto &v : values_) {
    if (js_->cmp(v, value) == 0)
      return true;
  }

  return false;
}

void
CJArray::
print(std::ostream &os) const
{
  os << "[";

  int i = 0;

  for (auto &v : values_) {
    if (i > 0)
      os << ",";

    os << " ";

    if (v)
      os << *v;

    ++i;
  }

  for (auto &kv : keyValues()) {
    if (i > 0)
      os << ",";

    os << " '" << kv.first << "': ";

    if (kv.second)
      os << *kv.second;
    else
      os << "<null>";

    ++i;
  }

  if (i > 0)
    os << " ";

  os << "]";
}


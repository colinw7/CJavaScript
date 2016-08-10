#include <CJArray.h>
#include <CJavaScript.h>

CJObjectTypeP CJArrayType::type_;

CJObjectTypeP
CJArrayType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CJArrayType(js));

    js->addObjectType("array", type_);

    js->addTypeObject(CJToken::Type::Array, type_);
  }

  return type_;
}

CJArrayType::
CJArrayType(CJavaScript *js) :
 CJObjectType(CJToken::Type::Array, "array")
{
  addFunction(js, "concat");
  addFunction(js, "join");
  addFunction(js, "pop");
  addFunction(js, "push");
  addFunction(js, "reverse");
  addFunction(js, "shift");
  addFunction(js, "sort");
  addFunction(js, "unshift");
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

  if      (name == "concat") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    if (values[1]->hasIndex()) {
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
  else if (name == "length") {
    long len = array->length();

    return js->createNumberValue(len);
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
  else {
    js->errorMsg("Invalid array function " + name);
  }

  return CJValueP();
}

//------

CJArray::
CJArray(CJavaScript *js, int n) :
 CJValue(CJArrayType::instance(js))
{
  if (n > 0)
    values_.resize(n);
}

CJArray::
CJArray(CJavaScript *js, const Values &values) :
 CJValue(CJArrayType::instance(js)), values_(values)
{
}

CJArray::
CJArray(CJavaScript *js, const std::vector<CJValueP> &values) :
 CJValue(CJArrayType::instance(js))
{
  for (const auto &v : values)
    values_.push_back(v);
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

CJValueP
CJArray::
indexValue(int i) const
{
  if (i < 0 || i >= int(values_.size()))
    return CJValueP();

  return values_[i];
}

void
CJArray::
setIndexValue(int i, CJValueP value)
{
  if (i < 0)
    return;

  while (i >= int(values_.size()))
    values_.push_back(CJValueP());

  values_[i] = value;
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
      return (v1->cmp(v2.get()) < 0);
    }
  };

  Cmp cmp;

  std::sort(values_.begin(), values_.end(), cmp);
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

  if (i > 0)
    os << " ";

  os << "]";
}


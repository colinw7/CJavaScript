#include <CJArray.h>
#include <CJFunction.h>
#include <CJavaScript.h>
#include <algorithm>

CJArrayTypeP CJArrayType::type_;

CJArrayTypeP
CJArrayType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJArrayTypeP(new CJArrayType(js));

    type_->init();

    js->addObjectType(type_->name(), type_);

    js->addTypeObject(CJToken::Type::Array, type_);
  }

  return type_;
}

CJArrayType::
CJArrayType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Array, "Array")
{
}

void
CJArrayType::
init()
{
  addTypeFunction(js_, "from");
  addTypeFunction(js_, "isArray");
  addTypeFunction(js_, "observe"); // obsolete
  addTypeFunction(js_, "of");
  addTypeFunction(js_, "unobserve"); // obsolete
  addTypeFunction(js_, "toString");

  addObjectFunction(js_, "concat"              , type_);
  addObjectFunction(js_, "forEach"             , type_);
  addObjectFunction(js_, "every"               , type_);
  addObjectFunction(js_, "filter"              , type_);
  addObjectFunction(js_, "hasOwnProperty"      , type_); // TODO: move to base class
  addObjectFunction(js_, "indexOf"             , type_);
  addObjectFunction(js_, "join"                , type_);
  addObjectFunction(js_, "lastIndexOf"         , type_);
  addObjectFunction(js_, "map"                 , type_);
  addObjectFunction(js_, "pop"                 , type_);
  addObjectFunction(js_, "propertyIsEnumerable", type_); // TODO: move to base class
  addObjectFunction(js_, "push"                , type_);
  addObjectFunction(js_, "reduce"              , type_);
  addObjectFunction(js_, "reduceRight"         , type_);
  addObjectFunction(js_, "reverse"             , type_);
  addObjectFunction(js_, "shift"               , type_);
  addObjectFunction(js_, "slice"               , type_);
  addObjectFunction(js_, "some"                , type_);
  addObjectFunction(js_, "splice"              , type_);
  addObjectFunction(js_, "sort"                , type_);
  addObjectFunction(js_, "toString"            , type_);
  addObjectFunction(js_, "toLocalString"       , type_);
  addObjectFunction(js_, "unshift"             , type_);
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

    if (js->isUndefinedOrNullValue(value1)) {
      js->throwTypeError(value1, "Cannot convert undefined or null to object");
      return CJValueP();
    }

    return createArrayFromValue(js, value1);
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
    CJArrayP array = js->createArrayValue();

    for (uint i = 1; i < values.size(); ++i)
      array->addValue(values[i]);

    return array;
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

bool
CJArrayType::
canCreateArrayFromValue(CJValueP value1)
{
  if (value1->hasIndex())
    return true;

  if (value1->hasProperty())
    return true;

  return false;
}

CJArrayP
CJArrayType::
createArrayFromValue(CJavaScript *js, CJValueP value1)
{
  CJArrayP array = js->createArrayValue();

  if      (value1->hasIndex()) {
    long len = value1->length();

    for (long ind = 0; ind < len; ++ind) {
      CJValueP ivalue = value1->indexValue(ind);

      if (ivalue)
        array->addValue(ivalue);
    }

    return array;
  }
  else if (value1->hasProperty()) {
    CJDictionaryP dict1 = CJValue::cast<CJDictionary>(value1);

    CJValueP lenValue = value1->propertyValue("length");

    if (lenValue) {
      int len = lenValue->toInteger();

      for (int ind = 0; ind < len; ++ind) {
        std::string istr = std::to_string(ind);

        CJValueP ivalue = value1->propertyValue(istr);

        if (ivalue)
          array->addValue(ivalue);
      }
    }
    else {
      CJValue::KeyNames names = value1->propertyNames();

      for (const auto &ind : names) {
        if (dict1 && ! dict1->isEnumerableProperty(ind))
          continue;

        CJValueP ivalue = value1->propertyValue(ind);

        if (ivalue)
          array->addValue(ivalue);
      }
    }
  }

  return array;
}

CJValueP
CJArrayType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  assert(values[0]->isArray());

  CJArrayP array = CJValue::cast<CJArray>(values[0]);

  // object functions
  if      (name == "concat") {
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
  else if (name == "every") {
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    CJDictionaryP thisDict;

    if (values.size() >= 3) {
      if (values[2]->isDictionary())
        thisDict = CJValue::cast<CJDictionary>(values[2]);
    }

    if (! fn) {
      js->errorMsg("Missing function for " + name);
      return CJValueP();
    }

    bool b = true;

    CJObjType::Values fnValues;

    fnValues.push_back(array);
    fnValues.push_back(CJValueP());
    fnValues.push_back(CJValueP());
    fnValues.push_back(array);

    for (int i = 0; i < array->length(); ++i) {
      fnValues[1] = array->indexValue(i);
      fnValues[2] = js->createNumberValue(long(i));

      CJValueP res;

      if (thisDict) {
        js->pushThis(thisDict);

        res = fn->exec(js, fnValues);

        js->popThis();
      }
      else {
        res = fn->exec(js, fnValues);
      }

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;

      if (! res->toBoolean()) {
        b = false;
        break;
      }
    }

    return js->createBoolValue(b);
  }
  else if (name == "filter") {
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    CJDictionaryP thisDict;

    if (values.size() >= 3) {
      if (values[2]->isDictionary())
        thisDict = CJValue::cast<CJDictionary>(values[2]);
    }

    if (! fn) {
      js->errorMsg("Missing function for " + name);
      return CJValueP();
    }

    std::vector<CJValueP> newValues;

    CJObjType::Values fnValues;

    fnValues.push_back(array);
    fnValues.push_back(CJValueP());
    fnValues.push_back(CJValueP());
    fnValues.push_back(array);

    for (int i = 0; i < array->length(); ++i) {
      fnValues[1] = array->indexValue(i);
      fnValues[2] = js->createNumberValue(long(i));

      CJValueP res;

      if (thisDict) {
        js->pushThis(thisDict);

        res = fn->exec(js, fnValues);

        js->popThis();
      }
      else {
        res = fn->exec(js, fnValues);
      }

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;

      if (res->toBoolean())
        newValues.push_back(fnValues[1]);
    }

    CJArrayP array1 = js->createArrayValue();

    array1->setValues(newValues);

    return array1;
  }
  else if (name == "forEach") {
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    CJDictionaryP thisDict;

    if (values.size() >= 3) {
      if (values[2]->isDictionary())
        thisDict = CJValue::cast<CJDictionary>(values[2]);
    }

    if (! fn) {
      js->errorMsg("Missing function for " + name);
      return CJValueP();
    }

    CJObjType::Values fnValues;

    fnValues.push_back(array);
    fnValues.push_back(CJValueP());
    fnValues.push_back(CJValueP());
    fnValues.push_back(array);

    for (int i = 0; i < array->length(); ++i) {
      fnValues[1] = array->indexValue(i);
      fnValues[2] = js->createNumberValue(long(i));

      CJValueP res;

      if (thisDict) {
        js->pushThis(thisDict);

        res = fn->exec(js, fnValues);

        js->popThis();
      }
      else {
        res = fn->exec(js, fnValues);
      }

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;
    }

    return CJValueP();
  }
  else if (name == "indexOf") {
    CJValueP search;

    if (values.size() >= 2)
      search = values[1];

    if (! search)
      return js->createNumberValue(-1L);

    int start = 0;
    int len   = array->length();

    if (values.size() >= 3) {
      start = values[2]->toInteger();

      if (start < 0)
        start = len + start;

      start = CJUtil::clamp(start, 0, len - 1);
    }

    for (int i = start; i < len; ++i) {
      COptInt cmp = js->cmp(array->indexValue(i), search);

      if (cmp.isValid() && cmp.getValue() == 0)
        return js->createNumberValue(long(i));
    }

    return js->createNumberValue(-1L);
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

      if (array->indexValue(i))
        str += array->indexValue(i)->toString();
    }

    return js->createStringValue(str);
  }
  else if (name == "lastIndexOf") {
    CJValueP search;

    if (values.size() >= 2)
      search = values[1];

    if (! search)
      return js->createNumberValue(-1L);

    int len   = array->length();
    int start = len - 1;

    if (values.size() >= 3) {
      start = values[2]->toInteger();

      if (start < 0)
        start = len + start;

      start = CJUtil::clamp(start, 0, len - 1);
    }

    for (int i = start; i >= 0; --i) {
      COptInt cmp = js->cmp(array->indexValue(i), search);

      if (cmp.isValid() && cmp.getValue() == 0)
        return js->createNumberValue(long(i));
    }

    return js->createNumberValue(-1L);
  }
  else if (name == "map") {
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    CJDictionaryP thisDict;

    if (values.size() >= 3) {
      if (values[2]->isDictionary())
        thisDict = CJValue::cast<CJDictionary>(values[2]);
    }

    if (! fn) {
      js->errorMsg("Missing function for " + name);
      return CJValueP();
    }

    std::vector<CJValueP> newValues;

    CJObjType::Values fnValues;

    fnValues.push_back(array);
    fnValues.push_back(CJValueP());
    fnValues.push_back(CJValueP());
    fnValues.push_back(array);

    for (int i = 0; i < array->length(); ++i) {
      fnValues[1] = array->indexValue(i);
      fnValues[2] = js->createNumberValue(long(i));

      CJValueP res;

      if (thisDict) {
        js->pushThis(thisDict);

        res = fn->exec(js, fnValues);

        js->popThis();
      }
      else {
        res = fn->exec(js, fnValues);
      }

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;

      newValues.push_back(res);
    }

    CJArrayP array1 = js->createArrayValue();

    array1->setValues(newValues);

    return array1;
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
  else if (name == "reduce") {
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    CJValueP ivalue;

    if (values.size() >= 3) {
      ivalue = values[2];
    }

    if (! fn) {
      js->errorMsg("Missing function for " + name);
      return CJValueP();
    }

    CJObjType::Values fnValues;

    fnValues.push_back(array);
    fnValues.push_back(ivalue);     // prevValue
    fnValues.push_back(CJValueP()); // curValue
    fnValues.push_back(CJValueP()); // currentIndex
    fnValues.push_back(array);      // array

    int i   = 0;
    int len = array->length();

    if (! ivalue && len)
      fnValues[1] = array->indexValue(i++);

    for ( ; i < len; ++i) {
      fnValues[2] = array->indexValue(i);           // curValue
      fnValues[3] = js->createNumberValue(long(i)); // currentIndex

      CJValueP res = fn->exec(js, fnValues);

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;

      fnValues[1] = res;
    }

    return fnValues[1];
  }
  else if (name == "reduceRight") {
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    CJValueP ivalue;

    if (values.size() >= 3) {
      ivalue = values[2];
    }

    if (! fn) {
      js->errorMsg("Missing function for " + name);
      return CJValueP();
    }

    CJObjType::Values fnValues;

    fnValues.push_back(array);
    fnValues.push_back(ivalue);     // prevValue
    fnValues.push_back(CJValueP()); // curValue
    fnValues.push_back(CJValueP()); // currentIndex
    fnValues.push_back(array);      // array

    int len = array->length();
    int i   = len - 1;

    if (! ivalue && len)
      fnValues[1] = array->indexValue(i--);

    for ( ; i >= 0; --i) {
      fnValues[2] = array->indexValue(i);           // curValue
      fnValues[3] = js->createNumberValue(long(i)); // currentIndex

      CJValueP res = fn->exec(js, fnValues);

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;

      fnValues[1] = res;
    }

    return fnValues[1];
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
  else if (name == "slice") {
    int len = array->length();

    int start = 0;
    int end   = len;

    if (values.size() >= 2) {
      start = values[1]->toInteger();

      if (start < 0)
        start = len + start;

      start = CJUtil::clamp(start, 0, len - 1);
    }

    if (values.size() >= 3) {
      end = values[2]->toInteger();

      if (end < 0)
        end = len + end;

      end = CJUtil::clamp(end, 0, len);
    }

    CJArrayP array1 = js->createArrayValue();

    for (int i = start; i < end; ++i) {
      array1->addValue(array->indexValue(i));
    }

    return array1;
  }
  else if (name == "some") {
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    CJDictionaryP thisDict;

    if (values.size() >= 3) {
      if (values[2]->isDictionary())
        thisDict = CJValue::cast<CJDictionary>(values[2]);
    }

    if (! fn) {
      js->errorMsg("Missing function for " + name);
      return CJValueP();
    }

    bool b = false;

    CJObjType::Values fnValues;

    fnValues.push_back(array);
    fnValues.push_back(CJValueP());
    fnValues.push_back(CJValueP());
    fnValues.push_back(array);

    for (int i = 0; i < array->length(); ++i) {
      fnValues[1] = array->indexValue(i);
      fnValues[2] = js->createNumberValue(long(i));

      CJValueP res;

      if (thisDict) {
        js->pushThis(thisDict);

        res = fn->exec(js, fnValues);

        js->popThis();
      }
      else {
        res = fn->exec(js, fnValues);
      }

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;

      if (res->toBoolean()) {
        b = true;
        break;
      }
    }

    return js->createBoolValue(b);
  }
  else if (name == "splice") {
    int len = array->length();

    int start  = 0;
    int numDel = 0;

    if (values.size() >= 2) {
      start = values[1]->toInteger();

      if (start < 0)
        start = len + start;

      start = CJUtil::clamp(start, 0, len - 1);
    }

    if (values.size() >= 3) {
      numDel = values[2]->toInteger();

      numDel = CJUtil::clamp(numDel, 0, len);
    }

    std::vector<CJValueP> newValues;
    std::vector<CJValueP> remValues;

    int i = 0;

    for ( ; i < start; ++i)
      newValues.push_back(array->indexValue(i));

    // save removed values
    for ( ; i < start + numDel && i < len; ++i)
      remValues.push_back(array->indexValue(i));

    // add new values
    for (std::size_t j = 3; j < values.size(); ++j)
      newValues.push_back(values[j]);

    // add remaining values
    for ( ; i < len; ++i)
      newValues.push_back(array->indexValue(i));

    // update array to new values
    array->setValues(newValues);

    // return array of old values
    CJArrayP array1 = js->createArrayValue();

    array1->setValues(remValues);

    return array1;
  }
  else if (name == "sort") {
    if (values.size() > 1) {
      // TODO: sort function name
    }

    array->sort();

    return values[0];
  }
  else if (name == "sum") {
  }
  else if (name == "toString") {
    return js->createStringValue(array->toString());
  }
  else if (name == "toLocaleString") {
    return js->createStringValue(array->toString());
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
  setValues(values);

  addPseudoProperty("length");
}

void
CJArray::
setValues(const std::vector<CJValueP> &values)
{
  values_.clear();

  int n = values.size();

  values_.resize(n);

  for (int ind = 0; ind < n; ++ind)
    values_[ind] = PropertyData(values[ind]);
}

std::string
CJArray::
toString() const
{
  std::string str;

  int i = 0;

  for (auto &v : values_) {
    if (i > 0)
      str += ",";

    if (v.value)
      str += v.value->toString();

    ++i;
  }

  for (auto &kv : keyValues()) {
    if (i > 0)
      str += ",";

    str += "'" + kv.first + "': ";

    if (kv.second.value)
      str += kv.second.value->toString();
    else
      str += "<null>";

    ++i;
  }

  return str;
}

void
CJArray::
addValue(CJValueP value)
{
  values_.push_back(PropertyData(value));
}

CJValueP
CJArray::
removeValue()
{
  assert(! values_.empty());

  PropertyData data = values_.back();

  values_.pop_back();

  return data.value;
}

void
CJArray::
addFrontValue(CJValueP value)
{
  int n = values_.size();

  values_.resize(n + 1);

  for (int i = n - 1; i >= 0; --i)
    values_[i + 1] = values_[i];

  values_[0] = PropertyData(value);
}

CJValueP
CJArray::
removeFrontValue()
{
  assert(! values_.empty());

  int n = values_.size();

  PropertyData data = values_[0];

  for (int i = 1; i < n; ++i)
    values_[i - 1] = values_[i];

  values_.pop_back();

  return data.value;
}

bool
CJArray::
hasIndexValue(int ind) const
{
  int n = values_.size();

  if (ind < 0 || ind >= n)
    return false;

  return !!values_[ind].value;
}

CJValueP
CJArray::
indexValue(int ind) const
{
  int n = values_.size();

  if (ind < 0 || ind >= n)
    return CJValueP();

  return values_[ind].value;
}

void
CJArray::
setIndexValue(int ind, CJValueP value)
{
  if (ind < 0)
    return;

  int n = values_.size();

  if (ind < n && ! isWritableIndex(ind))
    return;

  while (ind >= n) {
    values_.push_back(CJValueP());

    ++n;
  }

  values_[ind].value = value;
}

void
CJArray::
deleteIndexValue(int ind)
{
  setIndexValue(ind, CJValueP());
}

bool
CJArray::
isWritableIndex(int ind) const
{
  if (ind < 0 || ind >= int(values_.size()))
    return false;

  return values_[ind].writable.getValue(true);
}

void
CJArray::
setWritableIndex(int ind, bool b)
{
  assert(ind >= 0 && ind < int(values_.size()));

  values_[ind].writable = b;
}

bool
CJArray::
isEnumerableIndex(int ind) const
{
  if (ind < 0 || ind >= int(values_.size()))
    return false;

  return values_[ind].enumerable.getValue(true);
}

void
CJArray::
setEnumerableIndex(int ind, bool b)
{
  assert(ind >= 0 && ind < int(values_.size()));

  values_[ind].enumerable = b;
}

bool
CJArray::
isConfigurableIndex(int ind) const
{
  if (ind < 0 || ind >= int(values_.size()))
    return false;

  return values_[ind].configurable.getValue(true);
}

void
CJArray::
setConfigurableIndex(int ind, bool b)
{
  assert(ind >= 0 && ind < int(values_.size()));

  values_[ind].configurable = b;
}

void
CJArray::
reverse()
{
  int n = values_.size();

  for (int i = 0, j = n - 1; i < j; ++i, --j)
    std::swap(values_[i], values_[j]);
}

void
CJArray::
sort()
{
  struct Cmp {
    bool operator()(const PropertyData &v1, const PropertyData &v2) {
      if (! v2.value) return false;
      if (! v1.value) return true;
      return (v1.value->cmp(v2.value.get()) < 0);
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
    int len = value->toInteger();

    int n = values_.size();

    if (len < n) {
      while (n > len) {
        --n;

        values_.pop_back();
      }
    }
    else {
      while (n < len) {
        values_.push_back(PropertyData());

        ++n;
      }
    }
  }
  else
    CJObj::setProperty(js, key, value);
}

bool
CJArray::
hasValue(CJValueP value) const
{
  for (const auto &v : values_) {
    if (js_->cmp(v.value, value) == 0)
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

    if (v.value)
      os << *v.value;

    ++i;
  }

  for (auto &kv : keyValues()) {
    if (i > 0)
      os << ",";

    os << " '" << kv.first << "': ";

    if (kv.second.value)
      os << *kv.second.value;
    else
      os << "<null>";

    ++i;
  }

  if (i > 0)
    os << " ";

  os << "]";
}


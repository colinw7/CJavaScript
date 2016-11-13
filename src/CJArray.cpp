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
  addTypeFunction(js_, "from"     );
  addTypeFunction(js_, "isArray"  );
  addTypeFunction(js_, "observe"  ); // obsolete
  addTypeFunction(js_, "of"       );
  addTypeFunction(js_, "unobserve"); // obsolete
  addTypeFunction(js_, "toString" );

  addObjectFunction(js_, "concat"              , type_);
  addObjectFunction(js_, "every"               , type_);
  addObjectFunction(js_, "filter"              , type_);
  addObjectFunction(js_, "forEach"             , type_);
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
  addObjectFunction(js_, "toLocaleString"      , type_);
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
    long len = value1->length().getValue(0);

    for (long ind = 0; ind < len; ++ind) {
      CJValueP ivalue = value1->indexValue(ind);

      if (ivalue)
        array->addValue(ivalue);
    }

    return array;
  }
  else if (value1->hasProperty()) {
    CJDictionaryP dict1 = CJValue::cast<CJDictionary>(value1);

    COptLong lenValue = value1->length();

    if (lenValue.isValid()) {
      long len = lenValue.getValue(0);

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
          array->setProperty(js, ind, ivalue);
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

  // support array functions on non-array objects
  CJArrayP  array;
  CJObjectP obj;

  if      (values[0]->isArray())
    array = CJValue::cast<CJArray>(values[0]);
  else if (values[0]->isObject())
    obj = CJValue::cast<CJObject>(values[0]);

  if (! array && ! obj) {
    js->errorMsg("Invalid type for array function " + name);
    return CJValueP();
  }

  // object functions
  if      (name == "concat") {
    // create new array
    CJArrayP array1 = js->createArrayValue();

    int len1 = 0;

    // add all (expanded) values to array
    for (uint i = 0; i < values.size(); ++i) {
      if      (values[i]->isString()) { // special string handling (string has hasIndex() true)
        array1->setIndexValue(len1, values[i]);

        ++len1;
      }
      else if (values[i]->isArray()) {
        CJArrayP array2 = CJValue::cast<CJArray>(values[i]);

        for (const auto &v : array2->indValues()) {
          int ind1 = v.first + len1;

          array1->setIndexValue(ind1, v.second.calcValue(js));
        }

        len1 += array2->length().getValue(0);
      }
      else if (values[i]->hasIndex()) {
        long len = values[i]->length().getValue(0);

        for (long j = 0; j < len; ++j) {
          array1->setIndexValue(len1, values[i]->indexValue(j));

          ++len1;
        }
      }
      else {
        array1->setIndexValue(len1, values[i]);

        ++len1;
      }

      array1->setLength(len1);
    }

    return array1;
  }
  else if (name == "every") {
    // get function
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    if (! fn) {
      js->throwTypeError(values[0], "Missing function for " + name);
      return CJValueP();
    }

    //---

    // get optional this
    CJDictionaryP thisDict;

    if (values.size() >= 3) {
      if (values[2]->isDictionary())
        thisDict = CJValue::cast<CJDictionary>(values[2]);
    }

    //---

    // ensure first value is array
    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    //---

    // run function on each element
    bool b = true;

    if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
      return CJValueP();

    CJObjType::Values fnValues;

    fnValues.push_back(array);      // this
    fnValues.push_back(CJValueP()); // value
    fnValues.push_back(CJValueP()); // index
    fnValues.push_back(array);      // array

    for (const auto &v : array->indValues()) {
      fnValues[1] = v.second.calcValue(js);
      fnValues[2] = js->createNumberValue(v.first);

      CJValueP res = js->execFunction(fn, fnValues, thisDict);

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;

      if (! res->toBoolean()) {
        b = false;
        break;
      }
    }

    //---

    // return every check
    return js->createBoolValue(b);
  }
  else if (name == "filter") {
    // get function
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    if (! fn) {
      js->throwTypeError(values[0], "Missing function for " + name);
      return CJValueP();
    }

    //---

    // get optional this
    CJDictionaryP thisDict;

    if (values.size() >= 3) {
      if (values[2]->isDictionary())
        thisDict = CJValue::cast<CJDictionary>(values[2]);
    }

    //---

    // ensure first value is array
    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    //---

    // run function on each element
    std::vector<CJValueP> newValues;

    if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
      return CJValueP();

    CJObjType::Values fnValues;

    fnValues.push_back(array);      // this
    fnValues.push_back(CJValueP()); // value
    fnValues.push_back(CJValueP()); // index
    fnValues.push_back(array);      // array

    for (const auto &v : array->indValues()) {
      fnValues[1] = v.second.calcValue(js);
      fnValues[2] = js->createNumberValue(v.first);

      CJValueP res = js->execFunction(fn, fnValues, thisDict);

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;

      if (res->toBoolean())
        newValues.push_back(fnValues[1]);
    }

    //---

    // return filtered values
    CJArrayP array1 = js->createArrayValue();

    array1->setValues(newValues);

    return array1;
  }
  else if (name == "forEach") {
    // get function
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    if (! fn) {
      js->throwTypeError(values[0], "Missing function for " + name);
      return CJValueP();
    }

    //---

    // get optional this
    CJDictionaryP thisDict;

    if (values.size() >= 3) {
      if (values[2]->isDictionary())
        thisDict = CJValue::cast<CJDictionary>(values[2]);
    }

    //---

    // ensure first value is array
    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    //---

    // run function on each element
    if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
      return CJValueP();

    CJObjType::Values fnValues;

    fnValues.push_back(array);      // this
    fnValues.push_back(CJValueP()); // value
    fnValues.push_back(CJValueP()); // index
    fnValues.push_back(array);      // array

    for (const auto &v : array->indValues()) {
      fnValues[1] = v.second.calcValue(js);
      fnValues[2] = js->createNumberValue(v.first);

      CJValueP res = js->execFunction(fn, fnValues, thisDict);

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;
    }

    //---

    // return undefined
    return js->createUndefinedValue();
  }
  else if (name == "indexOf") {
    // get search value
    CJValueP search;

    if (values.size() >= 2)
      search = values[1];

    if (! search)
      return js->createNumberValue(-1L);

    //---

    // get input array
    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    //---

    long start = 0;
    long len   = array->length().getValue(0);

    //---

    // get start index
    if (values.size() >= 3) {
      COptLong optStart = values[2]->toInteger();

      if (! optStart.isValid()) {
        js->throwTypeError(values[0], "Invalid start index for " + name);
        return CJValueP();
      }

      start = optStart.getValue(0);

      if (start < 0)
        start = len + start;

      if (start >= len)
        return js->createNumberValue(-1L);

      start = CJUtil::clamp(start, 0L, len - 1);
    }

    //---

    // search
    for (long i = start; i < len; ++i) {
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

    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    std::string str;

    long len = array->length().getValue(0);

    for (long i = 0; i < len; ++i) {
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

    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    long len   = array->length().getValue(0);
    long start = len - 1;

    if (values.size() >= 3) {
      start = values[2]->toInteger().getValue(0);

      if (start < 0)
        start = len + start;

      if (start < 0)
        return js->createNumberValue(-1L);

      start = CJUtil::clamp(start, 0L, len - 1);
    }

    for (long i = start; i >= 0; --i) {
      CJValueP v = array->indexValue(i);

      COptInt cmp;

      if (v)
        cmp = js->cmp(v, search);

      if (cmp.isValid() && cmp.getValue() == 0)
        return js->createNumberValue(long(i));
    }

    return js->createNumberValue(-1L);
  }
  else if (name == "map") {
    // get function
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    if (! fn) {
      js->throwTypeError(values[0], "Missing function for " + name);
      return CJValueP();
    }

    //---

    // get optional this
    CJDictionaryP thisDict;

    if (values.size() >= 3) {
      if (values[2]->isDictionary())
        thisDict = CJValue::cast<CJDictionary>(values[2]);
    }

    if (! thisDict)
      thisDict = array;

    //---

    // ensure first value is array
    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    //---

    // run function on each element
    CJArray::IndValues newValues;

    if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
      return CJValueP();

    CJObjType::Values fnValues;

    fnValues.push_back(array);      // this
    fnValues.push_back(CJValueP()); // value
    fnValues.push_back(CJValueP()); // index
    fnValues.push_back(array);      // array

    for (const auto &v : array->indValues()) {
      fnValues[1] = v.second.calcValue(js);
      fnValues[2] = js->createNumberValue(v.first);

      CJValueP res;

      if (fnValues[1]) {
        res = js->execFunction(fn, fnValues, thisDict);

        if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
          break;
      }

      newValues[v.first] = res;
    }

    //---

    // return new values
    CJArrayP array1 = js->createArrayValue();

    array1->setValues(newValues, array->length().getValue(0));

    return array1;
  }
  else if (name == "pop") {
    CJValueP res;

    if (array) {
      long len = array->length().getValue(0);

      if (len < 1)
        return js->createUndefinedValue();

      res = array->removeValue();
    }
    else if (obj) {
      COptLong len = obj->length();

      long n = len.getValue(0);

      if (n > 0)
        res = obj->getProperty(js, std::to_string(n - 1));
      else
        res = js->createUndefinedValue();

      if (n > 0)
        obj->setLength(n - 1);
      else
        obj->setLength(0);
    }
    else {
      js->throwTypeError(values[0], "Invalid value type for " + name);
    }

    return res;
  }
  else if (name == "push") {
    long n = 0;

    if      (array) {
      for (uint i = 1; i < values.size(); ++i)
        array->addValue(values[i]);

      n = array->length().getValue(0);
    }
    else if (obj) {
      COptLong len = obj->length();

      n = len.getValue(0);

      for (uint i = 1; i < values.size(); ++i) {
        if (n >= CJUtil::maxInteger() + 1)
          n = 0;

        obj->setProperty(js, std::to_string(n), values[i]);

        ++n;
      }

      obj->setLength(n);
    }
    else {
      js->throwTypeError(values[0], "Invalid value type for " + name);
    }

    return js->createNumberValue(n);
  }
  else if (name == "reduce") {
    // get function
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    if (! fn) {
      js->throwTypeError(values[0], "Missing function for " + name);
      return CJValueP();
    }

    //---

    // get init value
    CJValueP ivalue;
    bool     needInit = true;

    if (values.size() >= 3) {
      ivalue   = values[2];
      needInit = false;
    }

    //---

    // get array to process
    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    //---

    CJObjType::Values fnValues;

    fnValues.push_back(array);
    fnValues.push_back(ivalue);     // prevValue
    fnValues.push_back(CJValueP()); // curValue
    fnValues.push_back(CJValueP()); // currentIndex
    fnValues.push_back(array);      // array

    for (const auto &v : array->indValues()) {
      if (needInit) {
        fnValues[1] = v.second.calcValue(js);
        needInit = false;
        continue;
      }

      fnValues[2] = v.second.calcValue(js);         // curValue
      fnValues[3] = js->createNumberValue(v.first); // currentIndex

      CJValueP res = fn->exec(js, fnValues);

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;

      fnValues[1] = res;
    }

    if (needInit) {
      js->throwTypeError(values[0], "Missing initial value for " + name);
      return CJValueP();
    }

    return fnValues[1];
  }
  else if (name == "reduceRight") {
    // get function
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    if (! fn) {
      js->throwTypeError(values[0], "Missing function for " + name);
      return CJValueP();
    }

    //---

    // get init value
    CJValueP ivalue;
    bool     needInit = true;

    if (values.size() >= 3) {
      ivalue = values[2];
      needInit = false;
    }

    //---

    // get array to process
    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    //---

    CJObjType::Values fnValues;

    fnValues.push_back(array);
    fnValues.push_back(ivalue);     // prevValue
    fnValues.push_back(CJValueP()); // curValue
    fnValues.push_back(CJValueP()); // currentIndex
    fnValues.push_back(array);      // array

    for (auto p = array->indValues().rbegin(); p != array->indValues().rend(); ++p) {
      if (needInit) {
        fnValues[1] = (*p).second.calcValue(js);
        needInit = false;
        continue;
      }

      fnValues[2] = (*p).second.calcValue(js);         // curValue
      fnValues[3] = js->createNumberValue((*p).first); // currentIndex

      CJValueP res = fn->exec(js, fnValues);

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;

      fnValues[1] = res;
    }

    if (needInit) {
      js->throwTypeError(values[0], "Missing initial value for " + name);
      return CJValueP();
    }

    return fnValues[1];
  }
  else if (name == "reverse") {
    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    array->reverse();

    return array;
  }
  else if (name == "shift") {
    CJValueP remVal;

    if      (array) {
      long len = array->length().getValue(0);

      if (len >= 1)
        remVal = array->removeFrontValue();
      else
        remVal = js->createUndefinedValue();
    }
    else if (obj) {
      COptLong len = obj->length();

      long n = len.getValue(0);

      if (n >= 1) {
        for (long i = 0; i < n; ++i) {
          std::string ind = std::to_string(i);

          CJValueP val = obj->getProperty(js, ind);

          if (i == 0)
            remVal = val;
          else {
            if (! val) continue;

            std::string ind1 = std::to_string(i - 1);

            obj->setProperty(js, ind1, val);

            obj->deleteProperty(js, ind);
          }
        }
      }
      else
        remVal = js->createUndefinedValue();

      obj->setLength(n - 1);
    }
    else {
      array = createArrayFromValue(js, values[0]);

      long len = array->length().getValue(0);

      if (len >= 1)
        remVal = array->removeFrontValue();
      else
        remVal = js->createUndefinedValue();
    }

    return remVal;
  }
  else if (name == "slice") {
    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    long len = array->length().getValue(0);

    long start = 0;
    long end   = len;

    if (values.size() >= 2) {
      COptLong optStart = values[1]->toInteger();

      if (optStart.isValid()) {
        start = optStart.getValue();

        if (start < 0)
          start = len + start;

        start = CJUtil::clamp(start, 0L, len);
      }
    }

    if (values.size() >= 3) {
      COptLong optEnd = values[2]->toInteger();

      if (optEnd.isValid()) {
        end = optEnd.getValue();

        if (end < 0)
          end = len + end;

        end = CJUtil::clamp(end, 0L, len);
      }
    }

    CJArray::IndValues newValues;
    long               newLen = 0;

    for (const auto &v : array->indValues()) {
      long ind = v.first;

      if (ind >= start && ind < end) {
        long ind1 = ind - start;

        newValues[ind1] = v.second;

        newLen = std::max(newLen, ind1 + 1);
      }
    }

    CJArrayP array1 = js->createArrayValue();

    array1->setValues(newValues, newLen);

    return array1;
  }
  else if (name == "some") {
    // get function
    CJFunctionP fn;

    if (values.size() >= 2) {
      if (values[1]->isFunction())
        fn = CJValue::cast<CJFunction>(values[1]);
    }

    if (! fn) {
      js->throwTypeError(values[0], "Missing function for " + name);
      return CJValueP();
    }

    //---

    // get optional this
    CJDictionaryP thisDict;

    if (values.size() >= 3) {
      if (values[2]->isDictionary())
        thisDict = CJValue::cast<CJDictionary>(values[2]);
    }

    //---

    // ensure first value is array
    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    //---

    // run function on each element
    bool b = false;

    if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
      return CJValueP();

    CJObjType::Values fnValues;

    fnValues.push_back(array);      // this
    fnValues.push_back(CJValueP()); // value
    fnValues.push_back(CJValueP()); // index
    fnValues.push_back(array);      // array

    for (const auto &v : array->indValues()) {
      fnValues[1] = v.second.calcValue(js);
      fnValues[2] = js->createNumberValue(v.first);

      CJValueP res = js->execFunction(fn, fnValues, thisDict);

      if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
        break;

      if (res->toBoolean()) {
        b = true;
        break;
      }
    }

    //---

    // return some check

    return js->createBoolValue(b);
  }
  else if (name == "splice") {
    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    long len = array->length().getValue(0);

    // get start of splice (default to end of array)
    long start = 0;

    if (values.size() >= 2) {
      start = values[1]->toInteger().getValue(0);

      if (start < 0)
        start = len + start;

      start = CJUtil::clamp(start, 0L, len);
    }
    else
      start = len;

    // get number of elements to delete (default to all elements after start)
    long numDel = 0;

    if (values.size() >= 3) {
      numDel = values[2]->toInteger().getValue(0);

      numDel = CJUtil::clamp(numDel, 0L, len);
    }
    else
      numDel = len - start;

    // get number to insert
    long numIns = std::max<long>(values.size() - 3, 0);

    //---

    CJArray::IndValues newValues, remValues;
    long               newLen = 0, remLen = 0;

    for (const auto &v : array->indValues()) {
      long ind = v.first;

      // add values before start
      if      (ind < start) {
        newValues[ind] = v.second;

        newLen = std::max<long>(newLen, ind + 1);
      }
      // add values after start + numDel
      else if (ind >= start + numDel) {
        long ind1 = ind - numDel + numIns;

        newValues[ind1] = v.second;

        newLen = std::max<long>(newLen, ind1 + 1);
      }
      // save removed values
      else {
        long ind1 = ind - start;

        remValues[ind1] = v.second;

        remLen = std::max<long>(remLen, ind1 + 1);
      }
    }

    // insert new values
    for (std::size_t j = 3; j < values.size(); ++j) {
      long ind1 = start + j - 3;

      newValues[ind1] = CJPropertyValue(values[j]);

      newLen = std::max<long>(newLen, ind1 + 1);
    }

    // update array to new values
    array->setValues(newValues, newLen);

    // return array of old values
    CJArrayP array1 = js->createArrayValue();

    array1->setValues(remValues, remLen);

    return array1;
  }
  else if (name == "sort") {
    CJFunctionP fn;

    if (values.size() > 1) {
      if (! values[1]->isFunction()) {
        js->throwTypeError(values[0], "Invalid function for " + name);
        return CJValueP();
      }

      fn = CJValue::cast<CJFunction>(values[1]);
    }

    if (! array)
      array = createArrayFromValue(js, values[0]);

    if (! array) {
      js->throwTypeError(values[0], "Missing array for " + name);
      return CJValueP();
    }

    array->sort(fn);

    return array;
  }
  else if (name == "sum") {
    assert(false);
  }
  else if (name == "toString") {
    CJValueP joinValue;

    if (values[0]->isDictionary()) {
      CJDictionaryP dict = CJValue::cast<CJDictionary>(values[0]);

      joinValue = dict->getProperty(js_, "join");
    }

    if (joinValue) {
      if (joinValue->isFunction()) {
        CJFunctionP joinFn = CJValue::cast<CJFunction>(joinValue);

        Values fnValues = values;

        CJValueP res = js->execFunction(joinFn, fnValues);

        return res;
      }
      else
        return CJObjType::exec(js, name, values);
    }
    else
      return js->createStringValue(values[0]->toString());
  }
  else if (name == "toLocaleString") {
    std::string str;

    if (array) {
      int i = 0;

      for (const auto &v : array->indValues()) {
        if (i > 0)
          str += ",";

        CJValueP value1 = v.second.calcValue(js);

        CJDictionaryP dictVal;

        if (value1 && value1->isDictionary())
          dictVal = CJValue::cast<CJDictionary>(value1);

        if      (dictVal) {
          CJValueP fnValue = dictVal->getProperty(js, "toLocaleString");

          if (fnValue) {
            if (fnValue->isFunction()) {
              CJFunctionBaseP fn = CJValue::cast<CJFunctionBase>(fnValue);

              Values fnValues;

              CJValueP res = js->execFunction(fn, fnValues);

              if (js->getCurrentBlock() && js->getCurrentBlock()->hasError())
                break;

              if (res)
                str += res->toString();
            }
            else {
              js->throwTypeError(values[0], "toLocaleString is not a function for " + name);
              return CJValueP();
            }
          }
          else
            str += dictVal->toString();
        }
        else if (value1) {
          if (value1->type() != CJToken::Type::Undefined &&
              value1->type() != CJToken::Type::Null)
            str += value1->toString();
        }

        ++i;
      }
    }

    return js->createStringValue(str);
  }
  else if (name == "unshift") {
    long numAdd = values.size() - 1;

    long res = 0;

    if      (array) {
      for (long i = numAdd; i >= 1; --i)
        array->addFrontValue(values[i]);

      res = array->length().getValue(0L);
    }
    else if (obj) {
      COptLong len = obj->length();

      if (len.isValid()) {
        long n = len.getValue();

        for (long i = n - 1; i >= 0; --i) {
          std::string ind = std::to_string(i);

          CJPropertyValue data;

          if (obj->getPropertyData(js, ind, data)) {
            std::string ind1 = std::to_string(i + numAdd);

            obj->setProperty(js, ind1, data.calcValue(js));

            if (i >= 0 && i < numAdd) {
              obj->setProperty(js, ind, values[i + 1]);
            }
            else {
              obj->deleteProperty(js, ind);
            }
          }
          else {
            if (i >= 0 && i < numAdd) {
              obj->setProperty(js, ind, values[i + 1]);
            }
          }
        }
      }

      obj->setLength(len.getValue(0) + numAdd);

      res = obj->length().getValue(0L);
    }
    else {
      array = createArrayFromValue(js, values[0]);

      for (size_t i = values.size() - 1; i >= 1; --i)
        array->addFrontValue(values[i]);

      res = array->length().getValue(0L);
    }

    return js->createNumberValue(res);
  }
  else if (name == "propertyIsEnumerable") {
    if (values.size() < 2)
      return js->createBoolValue(false);

    COptLong ind = values[1]->toInteger();

    bool b = false;

    if (array && ind.isValid())
      b = array->isEnumerableIndex(ind.getValue());

    return js->createBoolValue(b);
  }
  else if (name == "hasOwnProperty") {
    if (values.size() < 2)
      return js->createBoolValue(false);

    return js->createBoolValue(js->hasIndexValue(values[0], values[1]));
  }
  else {
    js->errorMsg("Invalid array object function " + name);
  }

  return CJValueP();
}

//------

CJArray::
CJArray(CJavaScript *js, long n) :
 CJObj(js, CJArrayType::instance(js))
{
  if (n > 0)
    len_ = n;

  addPseudoProperty("length");
}

CJArray::
CJArray(CJavaScript *js, const IndValues &values, long len) :
 CJObj(js, CJArrayType::instance(js)), values_(values), len_(len)
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

CJArray::
CJArray(const CJArray &array) :
 CJObj(array), values_(array.values_), len_(array.len_)
{
}

void
CJArray::
setValues(const std::vector<CJValueP> &values)
{
  values_.clear();

  len_ = values.size();

  for (long ind = 0; ind < len_; ++ind)
    values_[ind] = CJPropertyValue(values[ind]);
}

void
CJArray::
setValues(const IndValues &values, long len)
{
  values_ = values;
  len_    = len;
}

std::string
CJArray::
toString() const
{
  auto addValue = [] (std::string &str, long &i, CJValueP value) {
    if (i > 0)
      str += ", ";

    if (value)
      str += value->toString();

    ++i;
  };

  //---

  std::string str;

  long i = 0;

  for (const auto &v : values_) {
    while (i < v.first)
      addValue(str, i, CJValueP());

    addValue(str, i, v.second.calcValue(js_));
  }

  while (i < len_)
    addValue(str, i, CJValueP());

  //---

  for (auto &kv : keyValues()) {
    if (i > 0)
      str += ", ";

    str += "'" + kv.first + "': ";

    CJValueP value1 = kv.second.calcValue(js_);

    if (value1)
      str += value1->toString();
    else
      str += "<null>";

    ++i;
  }

  return str;
}

COptReal
CJArray::
toReal() const
{
  long n = length().getValue(0);

  if      (n == 0)
    return COptReal(0);
  else if (n == 1) {
    CJValueP value = indexValue(0);

    return (value ? COptReal(value->toReal()) : COptReal());
  }
  else
    return COptReal(CJUtil::getNaN());
}

COptLong
CJArray::
toInteger() const
{
  long n = length().getValue(0);

  if      (n == 0)
    return COptLong(0);
  else if (n == 1) {
    CJValueP value = indexValue(0);

    return (value ? COptLong(value->toInteger()) : COptLong());
  }
  else
    return COptLong(0);
}

void
CJArray::
addValue(CJValueP value)
{
  if (len_ + 1 > CJUtil::maxInteger()) {
    js_->throwRangeError(this, "Invalid array length");
    return;
  }

  values_[len_++] = CJPropertyValue(value);
}

CJValueP
CJArray::
removeValue()
{
  assert(len_ > 0);

  --len_;

  CJPropertyValue data = values_[len_];

  values_.erase(len_);

  return data.value;
}

void
CJArray::
addFrontValue(CJValueP value)
{
  IndValues newValues;

  for (const auto &v : values_) {
    long ind1 = v.first + 1;

    newValues[ind1] = v.second;
  }

  newValues[0] = CJPropertyValue(value);

  values_ = newValues;

  ++len_;
}

CJValueP
CJArray::
removeFrontValue()
{
  assert(len_ > 0);

  CJPropertyValue remData;

  IndValues newValues;

  for (const auto &v : values_) {
    long ind1 = v.first - 1;

    if (ind1 >= 0)
      newValues[ind1] = v.second;
    else
      remData = v.second;
  }

  values_ = newValues;

  --len_;

  return remData.value;
}

bool
CJArray::
hasIndexValue(long ind) const
{
  if (ind < 0 || ind >= len_)
    return false;

  auto p = values_.find(ind);

  if (p == values_.end())
    return false;

  CJValueP value1 = (*p).second.calcValue(js_);

  return !!value1;
}

CJValueP
CJArray::
indexValue(long ind) const
{
  if (ind < 0 || ind >= len_)
    return CJValueP();

  auto p = values_.find(ind);

  if (p == values_.end())
    return CJValueP();

  CJValueP value = (*p).second.calcValue(js_);

  if (value && value->type() == CJToken::Type::GetterSetter) {
    CJGetterSetterP gs = CJValue::cast<CJGetterSetter>(value);

    CJObjectP obj = CJValue::cast<CJObject>(gs->parent());

    js_->pushThis(obj);

    value = gs->getValue();

    js_->popThis();
  }

  return value;
}

void
CJArray::
setIndexValue(long ind, CJValueP value)
{
  if (ind < 0)
    return;

  if (ind < len_ && ! isWritableIndex(ind))
    return;

  values_[ind].value = value;

  len_ = std::max(ind + 1, len_);
}

void
CJArray::
deleteIndexValue(long ind)
{
  assert(ind >= 0 && ind < len_);

  values_.erase(ind);
}

bool
CJArray::
isWritableIndex(long ind) const
{
  if (ind < 0 || ind >= len_)
    return false;

  auto p = values_.find(ind);

  if (p == values_.end())
    return true;

  return (*p).second.writable.getValue(true);
}

void
CJArray::
setWritableIndex(long ind, bool b)
{
  assert(ind >= 0 && ind < len_);

  values_[ind].writable = b;
}

bool
CJArray::
isEnumerableIndex(long ind) const
{
  if (ind < 0 || ind >= len_)
    return false;

  auto p = values_.find(ind);

  if (p == values_.end())
    return false;

  return (*p).second.enumerable.getValue(true);
}

void
CJArray::
setEnumerableIndex(long ind, bool b)
{
  assert(ind >= 0 && ind < len_);

  values_[ind].enumerable = b;
}

bool
CJArray::
isConfigurableIndex(long ind) const
{
  if (ind < 0 || ind >= len_)
    return false;

  auto p = values_.find(ind);

  if (p == values_.end())
    return true;

  return (*p).second.configurable.getValue(true);
}

void
CJArray::
setConfigurableIndex(long ind, bool b)
{
  assert(ind >= 0 && ind < len_);

  values_[ind].configurable = b;
}

COptLong
CJArray::
length() const
{
  return COptLong(len_);
}

void
CJArray::
setLength(long n)
{
  if (n > CJUtil::maxInteger()) {
    js_->throwRangeError(this, "Invalid array length");
    return;
  }

  if (n < len_) {
    while (len_ > n) {
      --len_;

      values_.erase(len_);
    }
  }
  else
    len_ = n;
}

void
CJArray::
reverse()
{
  for (long i = 0, j = len_ - 1; i < j; ++i, --j)
    std::swap(values_[i], values_[j]);
}

void
CJArray::
sort(CJFunctionP fn)
{
  class Cmp {
   public:
    Cmp(CJavaScript *js, CJFunctionP fn) :
     js_(js), fn_(fn) {
    }

    bool operator()(const CJPropertyValue &v1, const CJPropertyValue &v2) {
      if (! v2.value) return true;
      if (! v1.value) return false;

      if (js_->isUndefinedValue(v2.value)) return true;
      if (js_->isUndefinedValue(v1.value)) return false;

      if (! fn_) {
        return (v1.value->cmp(v2.value.get()) < 0);
      }
      else {
        CJObjType::Values fnValues;

        fnValues.push_back(CJValueP());
        fnValues.push_back(v1.value);
        fnValues.push_back(v2.value);

        CJValueP res = fn_->exec(js_, fnValues);

        if (! res)
          return false;

        return (res->toInteger().getValue(0) < 0);
      }
    }

   private:
    CJavaScript *js_ { 0 };
    CJFunctionP  fn_;
  };

  //---

  Cmp cmp(js_, fn);

  Values values;

  values.reserve(values_.size());

  for (const auto &v : values_)
    values.push_back(v.second);

  std::sort(values.begin(), values.end(), cmp);

  int i = 0;

  for (auto &v : values_) {
    v.second = values[i++];
  }
}

CJValueP
CJArray::
getProperty(CJavaScript *js, const std::string &key) const
{
  if (key == "length")
    return js->createNumberValue(length().getValue(0));

  return CJObj::getProperty(js, key);
}

void
CJArray::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  if (key == "length") {
    setLength(value->toInteger().getValue(0));
  }
  else
    CJObj::setProperty(js, key, value);
}

bool
CJArray::
hasValue(CJValueP value) const
{
  for (const auto &v : values_) {
    if (js_->cmp(v.second.calcValue(js_), value) == 0)
      return true;
  }

  return false;
}

void
CJArray::
print(std::ostream &os) const
{
  auto addValue = [] (std::ostream &os, long &i, CJValueP value) {
    if (i > 0)
      os << ",";

    os << " ";

    if (value)
      value->print(os);

    ++i;
  };

  //---

  os << "[";

  long i = 0;

  for (const auto &v : values_) {
    while (i < v.first)
      addValue(os, i, CJValueP());

    addValue(os, i, v.second.calcValue(js_));
  }

  while (i < len_)
    addValue(os, i, CJValueP());

  //---

  for (auto &kv : keyValues()) {
    if (i > 0)
      os << ",";

    os << " '" << kv.first << "': ";

    CJValueP value1 = kv.second.calcValue(js_);

    if (value1)
      os << *value1;
    else
      os << "<null>";

    ++i;
  }

  //---

  if (i > 0)
    os << " ";

  os << "]";
}

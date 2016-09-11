#include <CJJSON.h>
#include <CJDictionary.h>
#include <CJavaScript.h>
#include <CJUtil.h>
#include <CUtf8.h>

CJJSON::
CJJSON(CJavaScript *js)
{
  // JSON Dictionary
  // TODO: mark as internal ? different behavior than user dictionary
  dict_ = CJDictionaryP(new CJDictionary(js));

  js->setProperty("JSON", std::static_pointer_cast<CJValue>(dict_));

  dict_->setFunctionProperty(js, CJFunctionP(new CJJSONParseFunction    (js)));
  dict_->setFunctionProperty(js, CJFunctionP(new CJJSONStringifyFunction(js)));
}

//------

CJValueP
CJJSONParseFunction::
exec(CJavaScript *js, const Values &values)
{
  // text [, reviver ]
  std::string str;

  if (values.size() >= 1)
    str = values[0]->toString();

  CJFunctionP reviver;

  if (values.size() >= 2) {
    reviver = js->valueToFunction(values[1]);
  }

  //---

  CJson json;

  CJson::Value *value;

  json.setStrict(true);

  if (! json.loadString(str, value)) {
    js->throwSyntaxError(this, "Unexpected token");
    return CJValueP();
  }

  CJValueP key = js->createStringValue("");

  return toValue(js, key, value, reviver);
}

CJValueP
CJJSONParseFunction::
toValue(CJavaScript *js, CJValueP key, CJson::Value *value, CJFunctionP reviver)
{
  CJValueP value1;

  if      (value->isString()) {
    value1 = js->createStringValue(value->cast<CJson::String>()->value());

    callFunc(js, key, value1, reviver);
  }
  else if (value->isNumber()) {
    value1 = js->createNumberValue(value->cast<CJson::Number>()->value());

    callFunc(js, key, value1, reviver);
  }
  else if (value->isTrue()) {
    value1 = js->createTrueValue();

    callFunc(js, key, value1, reviver);
  }
  else if (value->isFalse()) {
    value1 = js->createFalseValue();

    callFunc(js, key, value1, reviver);
  }
  else if (value->isNull()) {
    value1 = js->createNullValue();

    callFunc(js, key, value1, reviver);
  }
  else if (value->isObject()) {
    CJson::Object *obj = value->cast<CJson::Object>();

    CJDictionary *dict = new CJDictionary(js);

    std::vector<std::string> names;

    obj->getNames(names);

    for (const auto &name : names) {
      CJson::Value *value1;

      obj->getNamedValue(name, value1);

      CJValueP value2 = toValue(js, key, value1);

      dict->setProperty(js, name, value2);
    }

    value1 = CJValueP(dict);

    if (reviver) {
      for (const auto &name : dict->propertyNames()) {
        CJValueP value2 = dict->propertyValue(name);

        CJValueP key1 = js->createStringValue(name);

        CJValueP value3 = callFunc(js, key1, value2, reviver);

        if (value3)
          dict->setPropertyValue(name, value3);
      }

      callFunc(js, key, value1, reviver);
    }
  }
  else if (value->isArray()) {
    CJson::Array *array = value->cast<CJson::Array>();

    CJArray *array1 = new CJArray(js);

    std::vector<CJson::Value *> values;

    long i = 0;

    for (const auto &value : array->values()) {
      CJValueP key1 = js->createNumberValue(i);

      CJValueP value1 = toValue(js, key1, value, reviver);

      array1->addValue(value1);

      ++i;
    }

    value1 = CJValueP(array1);

    callFunc(js, key, value1, reviver);
  }

  return value1;
}

CJValueP
CJJSONParseFunction::
callFunc(CJavaScript *js, CJValueP key, CJValueP value, CJFunctionP reviver)
{
  if (! reviver) return CJValueP();

  std::vector<CJValueP> values = { CJValueP(), key, value };

  return reviver->exec(js, values);
}

//------

CJValueP
CJJSONStringifyFunction::
exec(CJavaScript *js, const Values &values)
{
  // value [, filter [, indent ]]
  CJValueP value;

  if (values.size() >= 1)
    value = values[0];

  replacer_ = CJFunctionP();
  filter_   = CJArrayP();

  if (values.size() >= 2 && values[1]) {
    if (values[1]->type() == CJToken::Type::Array)
      filter_ = std::static_pointer_cast<CJArray>(values[1]);
    else
      replacer_ = js->valueToFunction(values[1]);
  }

  CJValueP indent;

  if (values.size() >= 3)
    indent = values[2];

  //---

  // Check for toJSON function
  toJSON_ = CJFunctionP();

  if ( value && value->type() == CJToken::Type::Dictionary) {
    CJDictionaryP dict = std::static_pointer_cast<CJDictionary>(value);

    CJValueP value1 = dict->getProperty(js, "toJSON");

    if (value1)
      toJSON_ = js->valueToFunction(value1);
  }

  //---

  if (value->type() == CJToken::Type::Undefined)
    return js->createUndefinedValue();

  //---

  CJValueP key;

  ivalue_ = value;

  std::string str;
  bool        skip = false;

  if (! stringify(js, key, value, str, skip))
    return js->createUndefinedValue();

  return js->createStringValue(str);
}

bool
CJJSONStringifyFunction::
stringify(CJavaScript *js, CJValueP key, CJValueP value, std::string &str, bool &skip) const
{
  if (toJSON_) {
    CJFunction::Values values;

    values.push_back(CJValueP());
    values.push_back(value);

    CJValueP value1 = toJSON_->exec(js, values);

    if (value1)
      value = value1;
  }

  //---

  if (replacer_) {
    CJFunction::Values values;

    values.push_back(CJValueP());
    values.push_back(key);
    values.push_back(value);

    CJValueP value1 = replacer_->exec(js, values);

    if (value1)
      value = value1;
  }

  //---

  if (key && filter_) {
    if (! filter_->hasValue(key)) {
      skip = true;
      return true;
    }
  }

  //---

  if      (value->type() == CJToken::Type::Dictionary) {
    CJDictionaryP dict = std::static_pointer_cast<CJDictionary>(value);

    std::string str1;

    for (const auto &name : dict->propertyNames()) {
      CJValueP value = dict->propertyValue(name);

      if (! value || value->type() == CJToken::Type::Undefined)
        continue;

      //---

      CJValueP key = js->createStringValue(name);

      std::string str2;
      bool        skip = false;

      if (value != ivalue_) {
        if (! stringify(js, key, value, str2, skip))
          return false;

        if (skip)
          continue;
      }
      else
        str2 = "[Circular]";

      //---

      if (str1 != "")
        str1 += ",";

      str1 += "\"" + name + "\":" + str2;
    }

    str = "{" + str1 + "}";
  }
  else if (value->type() == CJToken::Type::Array) {
    CJArrayP array = std::static_pointer_cast<CJArray>(value);

    std::string str1;

    for (int i = 0; i < array->length(); ++i) {
      CJValueP value = array->indexValue(i);

      if (! value)
        continue;

      //---

      CJValueP key;

      std::string str2;
      bool        skip = false;

      if (value != ivalue_) {
        if (! stringify(js, key, value, str2, skip))
          return false;

        if (skip)
          continue;
      }
      else
        str2 = "[Circular]";

      //---

      if (str1 != "")
        str1 += ",";

      str1 += str2;
    }

    str = "[" + str1 + "]";
  }
  else if (value->type() == CJToken::Type::Number) {
    str = value->toString();
  }
  else if (value->type() == CJToken::Type::True) {
    str = "true";
  }
  else if (value->type() == CJToken::Type::False) {
    str = "false";
  }
  else if (value->type() == CJToken::Type::Null) {
    str = "null";
  }
  else if (value->type() == CJToken::Type::Undefined) {
    str = "null";
  }
  else if (value->type() == CJToken::Type::String) {
    std::string str1 = encodeString(value->toString());

    str = "\"" + str1 + "\"";
  }
  else {
    return false;
  }

  return true;
}

std::string
CJJSONStringifyFunction::
encodeString(const std::string &s) const
{
  static char buffer[16];

  std::string str;

  int  i   = 0;
  uint len = s.size();

  while (i < int(len)) {
    ulong c = CUtf8::readNextChar(s, i, len);

    if (c <= 0x7f) {
      if (c <= 0x1f) {
        if      (c == '\b')
          str += "\\b";
        else if (c == '\f')
          str += "\\f";
        else if (c == '\n')
          str += "\\n";
        else if (c == '\r')
          str += "\\r";
        else if (c == '\t')
          str += "\\t";
        else if (c == '\v')
          str += "\\v";
        else {
          int i = (c & 0xff);

          ::sprintf(buffer, "%02x", i);

          str += "\\x" + std::string(&buffer[0]);
        }
      }
      else {
        str += char(c);
      }
    }
    else {
      int i1 = (c >> 12) & 0xF;
      int i2 = (c >> 8 ) & 0xF;
      int i3 = (c >> 4 ) & 0xF;
      int i4 =  c        & 0xF;

      std::string str2;

      ::sprintf(buffer, "%x", i1); str2 += buffer;
      ::sprintf(buffer, "%x", i2); str2 += buffer;
      ::sprintf(buffer, "%x", i3); str2 += buffer;
      ::sprintf(buffer, "%x", i4); str2 += buffer;

      str += "\\u" + str2;
    }
  }

  return str;
}

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

  js->setProperty("JSON", dict_);

  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJJSONFunction(js, "parse")));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJJSONFunction(js, "stringify")));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJJSONFunction(js, "toString")));
}

//------

CJValueP
CJJSONFunction::
exec(CJavaScript *js, const Values &values)
{
  if (name_ == "parse") {
    // text [, reviver ]
    std::string str;

    if (values.size() >= 1)
      str = values[0]->toString();

    CJFunctionBaseP reviver;

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

    return parseToValue(js, key, value, reviver);
  }
  else if (name_ == "stringify") {
    // value [, filter [, indent ]]
    CJValueP value;

    if (values.size() >= 1)
      value = values[0];

    replacer_ = CJFunctionBaseP();
    filter_   = CJArrayP();

    if (values.size() >= 2 && values[1]) {
      if (values[1]->type() == CJToken::Type::Array)
        filter_ = CJValue::cast<CJArray>(values[1]);
      else
        replacer_ = js->valueToFunction(values[1]);
    }

    Indent indent;

    if (values.size() >= 3) {
      // ignore bad types
      if      (values[2]->type() == CJToken::Type::Number) {
        CJNumberP number = CJValue::cast<CJNumber>(values[2]);

        long i = number->toInteger();

        if (i > 0)
          indent.setInteger(std::min(i, 10L));
      }
      else if (values[2]->type() == CJToken::Type::String) {
        CJStringP str = CJValue::cast<CJString>(values[2]);

        int len = str->length();

        if (len > 0) {
          std::string str1;

          if (len > 10)
            str1 = str->substr(0, 10);
          else
            str1 = str->text();

          indent.setString(str1);
        }
      }
    }

    //---

    // Check for toJSON function
    toJSON_ = CJFunctionBaseP();

    if (value && (value->type() == CJToken::Type::Object ||
                  value->type() == CJToken::Type::Dictionary)) {
      CJValueP value1;

      if (value->type() == CJToken::Type::Object) {
        CJObjectP obj = CJValue::cast<CJObject>(value);

        value1 = obj->getProperty(js, "toJSON");
      }
      else {
        CJDictionaryP dict = CJValue::cast<CJDictionary>(value);

        value1 = dict->getProperty(js, "toJSON");
      }

      if (value1)
        toJSON_ = js->valueToFunction(value1);
    }

    //---

    if (js->isUndefinedValue(value))
      return js->createUndefinedValue();

    //---

    CJValueP key;

    ivalue_ = value;

    std::string str;
    bool        skip = false;

    if (! stringify(js, key, value, indent, 1, str, skip))
      return js->createUndefinedValue();

    return js->createStringValue(str);
  }
  else if (name_ == "toString") {
    return js->createStringValue("[object JSON]");
  }
  else
    return CJValueP();
}

CJValueP
CJJSONFunction::
parseToValue(CJavaScript *js, CJValueP key, CJson::Value *value, CJFunctionBaseP reviver)
{
  CJValueP value1;

  if      (value->isString()) {
    value1 = js->createStringValue(value->cast<CJson::String>()->value());

    parseCallFunc(js, key, value1, reviver);
  }
  else if (value->isNumber()) {
    value1 = js->createNumberValue(value->cast<CJson::Number>()->value());

    parseCallFunc(js, key, value1, reviver);
  }
  else if (value->isTrue()) {
    value1 = js->createTrueValue();

    parseCallFunc(js, key, value1, reviver);
  }
  else if (value->isFalse()) {
    value1 = js->createFalseValue();

    parseCallFunc(js, key, value1, reviver);
  }
  else if (value->isNull()) {
    value1 = js->createNullValue();

    parseCallFunc(js, key, value1, reviver);
  }
  else if (value->isObject()) {
    CJson::Object *obj = value->cast<CJson::Object>();

    CJDictionary *dict = new CJDictionary(js);

    std::vector<std::string> names;

    obj->getNames(names);

    for (const auto &name : names) {
      CJson::Value *value1;

      obj->getNamedValue(name, value1);

      CJValueP value2 = parseToValue(js, key, value1);

      dict->setProperty(js, name, value2);
    }

    value1 = CJValueP(dict);

    if (reviver) {
      for (const auto &name : dict->propertyNames()) {
        CJValueP value2 = dict->propertyValue(name);

        CJValueP key1 = js->createStringValue(name);

        CJValueP value3 = parseCallFunc(js, key1, value2, reviver);

        if (value3)
          dict->setPropertyValue(name, value3);
      }

      parseCallFunc(js, key, value1, reviver);
    }
  }
  else if (value->isArray()) {
    CJson::Array *array = value->cast<CJson::Array>();

    CJArrayP array1 = js->createArrayValue();

    std::vector<CJson::Value *> values;

    long i = 0;

    for (const auto &value : array->values()) {
      CJValueP key1 = js->createNumberValue(i);

      CJValueP value1 = parseToValue(js, key1, value, reviver);

      array1->addValue(value1);

      ++i;
    }

    value1 = array1;

    parseCallFunc(js, key, array1, reviver);
  }

  return value1;
}

CJValueP
CJJSONFunction::
parseCallFunc(CJavaScript *js, CJValueP key, CJValueP value, CJFunctionBaseP reviver)
{
  if (! reviver) return CJValueP();

  std::vector<CJValueP> values = { CJValueP(), key, value };

  return reviver->exec(js, values);
}

bool
CJJSONFunction::
stringify(CJavaScript *js, CJValueP key, CJValueP value, const Indent &indent,
          int depth, std::string &str, bool &skip) const
{
  if (toJSON_) {
    CJFunctionBase::Values values;

    values.push_back(CJValueP());
    values.push_back(value);

    CJValueP value1 = toJSON_->exec(js, values);

    if (value1)
      value = value1;
  }

  //---

  if (key && replacer_) {
    CJFunctionBase::Values values;

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

  if      (value->type() == CJToken::Type::Object) {
    CJObjectP obj = CJValue::cast<CJObject>(value);

    std::string str1;

    for (const auto &name : obj->propertyNames()) {
      if (! obj->isEnumerableProperty(name))
        continue;

      CJValueP value = obj->propertyValue(name);

      if (js->isUndefinedValue(value))
        continue;

      //---

      CJValueP key = js->createStringValue(name);

      std::string str2;
      bool        skip = false;

      if (value != ivalue_) {
        if (! stringify(js, key, value, indent, depth + 1, str2, skip))
          continue;

        if (skip)
          continue;
      }
      else
        str2 = "[Circular]";

      //---

      if (str1 != "") {
        str1 += ",";

        if (indent.isValid())
          str1 += "\n" + indent.depthStr(depth);
      }

      if (indent.isValid())
        str1 += "\"" + name + "\": " + str2;
      else
        str1 += "\"" + name + "\":" + str2;
    }

    if (indent.isValid())
      str = "{\n" + indent.depthStr(depth) + str1 + "\n" + indent.depthStr(depth - 1) + "}";
    else
      str = "{" + str1 + "}";
  }
  else if (value->type() == CJToken::Type::Dictionary) {
    CJDictionaryP dict = CJValue::cast<CJDictionary>(value);

    std::string str1;

    for (const auto &name : dict->propertyNames()) {
      if (! dict->isEnumerableProperty(name))
        continue;

      CJValueP value = dict->propertyValue(name);

      if (js->isUndefinedValue(value))
        continue;

      //---

      CJValueP key = js->createStringValue(name);

      std::string str2;
      bool        skip = false;

      if (value != ivalue_) {
        if (! stringify(js, key, value, indent, depth + 1, str2, skip))
          return false;

        if (skip)
          continue;
      }
      else
        str2 = "[Circular]";

      //---

      if (str1 != "") {
        str1 += ",";

        if (indent.isValid())
          str1 += "\n" + indent.depthStr(depth);
      }

      if (indent.isValid())
        str1 += "\"" + name + "\": " + str2;
      else
        str1 += "\"" + name + "\":" + str2;
    }

    if (indent.isValid())
      str = "{\n" + indent.depthStr(depth) + str1 + "\n" + indent.depthStr(depth - 1) + "}";
    else
      str = "{" + str1 + "}";
  }
  else if (value->type() == CJToken::Type::Array) {
    CJArrayP array = CJValue::cast<CJArray>(value);

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
        if (! stringify(js, key, value, indent, depth + 1, str2, skip))
          return false;

        if (skip)
          continue;
      }
      else
        str2 = "[Circular]";

      //---

      if (str1 != "") {
        str1 += ",";

        if (indent.isValid())
          str1 += "\n" + indent.depthStr(depth);
      }

      str1 += str2;
    }

    if (indent.isValid())
      str = "[\n" + indent.depthStr(depth) + str1 + "\n" + indent.depthStr(depth - 1) + "]";
    else
      str = "[" + str1 + "]";
  }
  else if (value->isNumber()) {
    str = CJValue::cast<CJNumber>(value)->realString();
  }
  else if (value->type() == CJToken::Type::Boolean) {
    str = (value->toBoolean() ? "true" : "false");
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
    str = encodeString(CJValue::cast<CJString>(value)->text());
  }
  else {
    return false;
  }

  return true;
}

std::string
CJJSONFunction::
encodeString(const std::string &s) const
{
  static char buffer[16];

  std::string str;

  str += '"';

  int  i   = 0;
  uint len = s.size();

  while (i < int(len)) {
    ulong c = CUtf8::readNextChar(s, i, len);

    if (c <= 0x7f) {
      char c1 = char(c);

      if      (c1 <= 0x1f) {
        if      (c1 == '\b')
          str += "\\b";
        else if (c1 == '\f')
          str += "\\f";
        else if (c1 == '\n')
          str += "\\n";
        else if (c1 == '\r')
          str += "\\r";
        else if (c1 == '\t')
          str += "\\t";
        else if (c1 == '\v')
          str += "\\v";
        else {
          ::sprintf(buffer, "%02x", int(c1));

          str += "\\x" + std::string(&buffer[0]);
        }
      }
      else if (c1 == '\'' || c1 == '"') {
        str += '\\';

        str += c1;
      }
      else {
        str += c1;
      }
    }
    else {
      int len1;

      CUtf8::encode(c, buffer, len1);

      for (int j = 0; j < len1; ++j)
        str += buffer[j];

#if 0
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
#endif
    }
  }

  str += '"';

  return str;
}

std::string
CJJSONFunction::
toString() const
{
  std::stringstream ss;

  ss << *this;

  return ss.str();
}

void
CJJSONFunction::
print(std::ostream &os) const
{
  os << name_;
}

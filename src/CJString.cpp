#include <CJString.h>
#include <CJRegExp.h>
#include <CJavaScript.h>
#include <CJUtil.h>
#include <CUtf8.h>
#include <cstring>

CJStringTypeP CJStringType::type_;

CJObjTypeP
CJStringType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJStringTypeP(new CJStringType(js));

    type_->init();

    js->addObjectType(type_->name(), type_);

    js->addTypeObject(CJToken::Type::String, type_);
  }

  return type_;
}

CJStringType::
CJStringType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::String, "String")
{
}

void
CJStringType::
init()
{
  // String properties
  //   length, fromCharCode, fromCodePoint, raw
  // Function
  //   arguments, caller, displayName, length, name, prototype
  // Object
  //   __proto__, constructor
  addTypeFunction(js_, "fromCharCode" , type_);
  addTypeFunction(js_, "fromCodePoint", type_);
  addTypeFunction(js_, "raw"          , type_);
  addTypeFunction(js_, "toString"     , type_);

  // String.prototype properties
  //  anchor, charAt, charCodeAt, codePointAt, concat, endsWith,
  //  includes, indexOf, lastIndexOf, link, localCompare, match,
  //  normalize, repeat, replace, search, slice, split, startsWith,
  //  substr, substring, toLocaleLowerCase, toLocaleUpperCase,
  //  toLowerCase, toString, toUpperCase, trim, trimLeft, trimRight,
  // Function.prototype properties
  //  apply, bind, call, isGenerator, toSource, toString
  // Object.prototype properties
  //  hasOwnProperty, isPrototypeOf, propertyIsEnumerable,
  //  toLocalString,  toSource, toString, unwatch, valueOf, watch
  addObjectFunction(js_, "anchor"           , type_);
  addObjectFunction(js_, "charAt"           , type_);
  addObjectFunction(js_, "charCodeAt"       , type_);
  addObjectFunction(js_, "codePointAt"      , type_);
  addObjectFunction(js_, "concat"           , type_);
  addObjectFunction(js_, "endsWith"         , type_);
  addObjectFunction(js_, "includes"         , type_);
  addObjectFunction(js_, "indexOf"          , type_);
  addObjectFunction(js_, "lastIndexOf"      , type_);
  addObjectFunction(js_, "link"             , type_);
  addObjectFunction(js_, "localeCompare"    , type_);
  addObjectFunction(js_, "match"            , type_);
  addObjectFunction(js_, "normalize"        , type_);
  addObjectFunction(js_, "repeat"           , type_);
  addObjectFunction(js_, "replace"          , type_);
  addObjectFunction(js_, "search"           , type_);
  addObjectFunction(js_, "slice"            , type_);
  addObjectFunction(js_, "split"            , type_);
  addObjectFunction(js_, "startsWith"       , type_);
  addObjectFunction(js_, "substr"           , type_);
  addObjectFunction(js_, "substring"        , type_);
  addObjectFunction(js_, "toLocaleLowerCase", type_);
  addObjectFunction(js_, "toLocaleUpperCase", type_);
  addObjectFunction(js_, "toLowerCase"      , type_);
  addObjectFunction(js_, "toString"         , type_);
  addObjectFunction(js_, "toUpperCase"      , type_);
  addObjectFunction(js_, "trimLeft"         , type_);
  addObjectFunction(js_, "trimRight"        , type_);
  addObjectFunction(js_, "valueOf"          , type_);
}

CJValueP
CJStringType::
execType(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  // values[0] is CJObjectTypeFunction
  CJFunctionBaseP fn;

  if (values[0] && values[0]->isFunction())
    fn = CJValue::cast<CJFunctionBase>(values[0]);

  // type functions
  if      (name == "fromCharCode") {
    std::string str;

    for (uint i = 1; i < values.size(); ++i) {
      long code = values[i]->toInteger();

      CUtf8::append(str, code);
    }

    return js->createStringValue(str);
  }
  else if (name == "fromCodePoint") {
    std::string str;

    for (uint i = 1; i < values.size(); ++i) {
      long code = values[i]->toInteger();

      CUtf8::append(str, code);
    }

    return js->createStringValue(str);
  }
  else if (name == "raw") {
    return CJValueP();
  }
  else if (name == "toString") {
    if (fn)
      return js->createStringValue(fn->toString());
    else
      return js->createStringValue("function String() { }");
  }
  else {
    js->errorMsg("Invalid string function " + name);
  }

  return CJValueP();
}

CJValueP
CJStringType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJStringP cstr = CJValue::cast<CJString>(values[0]);
  assert(cstr);

  //---

  // object functions
  if      (name == "anchor") {
    return CJValueP();
  }
  else if (name == "charAt") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    long ind = (values[1] ? values[1]->toInteger() : 0);

    if (ind >= 0 && ind < cstr->length()) {
      return js->createStringValue(cstr->substr(ind, 1));
    }
  }
  else if (name == "charCodeAt") {
    return CJValueP();
  }
  else if (name == "codePointAt") {
    return CJValueP();
  }
  else if (name == "concat") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string str1 = (values[1] ? values[1]->toString() : std::string());

    std::string str = cstr->text();

    return js->createStringValue(str + str1);
  }
  else if (name == "endsWith") {
    return CJValueP();
  }
  else if (name == "includes") {
    return CJValueP();
  }
  else if (name == "indexOf") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    CJStringP key;

    if (values[1] && values[1]->type() == CJToken::Type::String)
      key = CJValue::cast<CJString>(values[1]);

    if (! key) {
      js->errorMsg("Invalid key for " + name);
      return CJValueP();
    }

    std::string str = cstr->text();

    auto p = str.find(key->text());

    if (p == std::string::npos)
      return js->createNumberValue(long(-1));

    return js->createNumberValue(long(p));
  }
  else if (name == "lastIndexOf") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    CJStringP key;

    if (values[1] && values[1]->type() == CJToken::Type::String)
      key = CJValue::cast<CJString>(values[1]);

    if (! key) {
      js->errorMsg("Invalid key for " + name);
      return CJValueP();
    }

    std::string str = cstr->text();

    auto p = str.rfind(key->text());

    if (p == std::string::npos)
      return js->createNumberValue(long(-1));

    return js->createNumberValue(long(p));
  }
  else if (name == "link") {
    return CJValueP();
  }
  else if (name == "localeCompare") {
    return CJValueP();
  }
  else if (name == "match") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    CJValueP rvalue = values[1];

    if (rvalue->type() != CJToken::Type::RegExp)
      rvalue = CJValueP(new CJRegExp(js, rvalue->toString()));

    CJRegExpP regexp = CJValue::cast<CJRegExp>(rvalue);

    std::string str = cstr->text();

    CJRegExp::MatchData data;

    bool b = regexp->find(str, data);

    return js->createBoolValue(b);
  }
  else if (name == "normalize") {
    return CJValueP();
  }
  else if (name == "repeat") {
    return CJValueP();
  }
  else if (name == "replace") {
    if (values.size() != 3) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string str1 = (values[1] ? values[1]->toString() : std::string());
    std::string str2 = (values[2] ? values[2]->toString() : std::string());

    std::string str = cstr->text();

    auto pos = str.find(str1);

    if (pos != std::string::npos)
      str.replace(pos, str1.length(), str2);

    return js->createStringValue(str);
  }
  else if (name == "search") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    CJValueP rvalue = values[1];

    if (rvalue->type() != CJToken::Type::RegExp)
      rvalue = CJValueP(new CJRegExp(js, rvalue->toString()));

    CJRegExpP regexp = CJValue::cast<CJRegExp>(rvalue);

    std::string str = cstr->text();

    CJRegExp::MatchData data;

    if (! regexp->find(str, data) || data.ranges.empty())
      return js->createNumberValue(-1L);

    return js->createNumberValue(long(data.ranges[0].first));
  }
  else if (name == "slice") {
    if (values.size() != 2 && values.size() != 3) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    long ind1 = (values[1] ? values[1]->toInteger() : 0);

    if (values.size() == 3) {
      long ind2 = (values[2] ? values[2]->toInteger() : 0);

      int len = cstr->length();

      if (ind2 < 0)
        ind2 = len + ind2;

      if ((ind1 >= 0 && ind1 < len) && (ind2 >= 0 && ind2 < len) && ind1 <= ind2) {
        std::string str1 = cstr->substr(ind1, ind2 - ind1);

        return js->createStringValue(str1);
      }
    }
    else {
      int len = cstr->length();

      if (ind1 >= 0 && ind1 < len) {
        return js->createStringValue(cstr->substr(ind1));
      }
    }
  }
  else if (name == "split") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string str = cstr->text();

    std::vector<std::string> strs;

    std::string sstr = (values[1] ? values[1]->toString() : std::string());

    if (! sstr.empty()) {
      auto p = str.find(sstr);

      while (p != std::string::npos) {
        strs.push_back(str.substr(0, p));

        str = str.substr(p + sstr.size());

        p = str.find(sstr);
      }

      strs.push_back(str);
    }
    else
      strs.push_back(str);

    //---

    CJArrayP array = js->createArrayValue();

    for (const auto &s : strs) {
      CJValueP value = js->createStringValue(s);

      array->addValue(value);
    }

    return array;
  }
  else if (name == "startsWith") {
    return CJValueP();
  }
  else if (name == "substr") {
    if (values.size() != 2 && values.size() != 3) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    long start = (values[1] ? values[1]->toInteger() : 0);

    if (values.size() == 3) {
      long len1 = (values[2] ? values[2]->toInteger() : 0);

      long end = start + len1;

      int len = cstr->length();

      if ((start >= 0 && start < len) && (end >= 0 && end < len) && start <= end) {
        return js->createStringValue(cstr->substr(start, end - start));
      }
    }
    else {
      int len = cstr->length();

      if (start >= 0 && start < len) {
        return js->createStringValue(cstr->substr(start));
      }
    }
  }
  else if (name == "substring") {
    if (values.size() != 2 && values.size() != 3) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    long ind1 = (values[1] ? values[1]->toInteger() : 0);

    if (values.size() == 3) {
      long ind2 = (values[2] ? values[2]->toInteger() : 0);

     int len = cstr->length();

      if ((ind1 >= 0 && ind1 < len) && (ind2 >= 0 && ind2 < len) && ind1 <= ind2) {
        return js->createStringValue(cstr->substr(ind1, ind2 - ind1));
      }
    }
    else {
     int len = cstr->length();

      if (ind1 >= 0 && ind1 < len) {
        return js->createStringValue(cstr->substr(ind1));
      }
    }
  }
  else if (name == "toLocalLowerCase") {
    return CJValueP();
  }
  else if (name == "toLocalUpperCase") {
    return CJValueP();
  }
  else if (name == "toLowerCase") {
    std::string str = cstr->text();

    int pos = 0;
    int len = str.size();

    std::string str1;

    while (pos < len) {
      ulong c = CUtf8::readNextChar(str, pos, len);

      if (c < 0x7f)
        c = tolower(c);

      CUtf8::append(str1, c);
    }

    return js->createStringValue(str1);
  }
  else if (name == "toString") {
    std::string str = cstr->text();

    return js->createStringValue(str);
  }
  else if (name == "toUpperCase") {
    std::string str = cstr->text();

    int pos = 0;
    int len = str.size();

    std::string str1;

    while (pos < len) {
      ulong c = CUtf8::readNextChar(str, pos, len);

      if (c < 0x7f)
        c = toupper(c);

      CUtf8::append(str1, c);
    }

    return js->createStringValue(str1);
  }
  else if (name == "trim") {
    return CJValueP();
  }
  else if (name == "trimLeft") {
    return CJValueP();
  }
  else if (name == "trimRight") {
    return CJValueP();
  }
  else if (name == "valueOf") {
    return CJValueP();
  }
  else {
    js->errorMsg("Invalid string function " + name);
  }

  return CJValueP();
}

//------

CJString::
CJString(CJavaScript *js, const std::string &text, char c) :
 CJObj(js, CJStringType::instance(js)), text_(text), c_(c)
{
  addPseudoProperty("length");
}

CJValueP
CJString::
getProperty(CJavaScript *js, const std::string &key) const
{
  if (key == "length")
    return js->createNumberValue(length());

  return CJObj::getProperty(js, key);
}

void
CJString::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  if (key == "length") {
    // TODO:
    //return CJValueP();
  }

  CJObj::setProperty(js, key, value);
}

double
CJString::
toReal() const
{
  COptReal real = parseFloat(text_);

  return real.getValue(CJUtil::getNaN());
}

long
CJString::
toInteger() const
{
  COptLong integer = parseInt(text_);

  return integer.getValue(0);
}

bool
CJString::
toBoolean() const
{
  COptBool b = parseBool(text_);

  return b.getValue(false);
}

COptReal
CJString::
parseFloat(const std::string &text, bool extraChars)
{
  // skip leading space
  int pos = 0;
  int len = text.size();

  while (pos < len) {
    int pos1 = pos;

    ulong c = CUtf8::readNextChar(text, pos1, len);

    if (! CUtf8::isSpace(c))
      break;

    pos = pos1;
  }

  // Fail on empty string
  if (pos >= len)
    return COptReal();

  //---

  const char *p1 = &text[pos];

  if (strncmp(p1, "NaN", 3) == 0)
    return COptReal(CJUtil::getPosInf());

  if (strncmp(p1, "Infinity", 8) == 0)
    return COptReal(CJUtil::getPosInf());

  if (strncmp(p1, "-Infinity", 9) == 0)
    return COptReal(CJUtil::getNegInf());

  const char *p2;

  errno = 0;

  double real = strtod(p1, (char **) &p2);

  if (errno == ERANGE)
    return COptReal();

  if (p1 == p2)
    return COptReal();

  while (*p2 != 0 && ::isspace(*p2))
    ++p2;

  if (*p2 != '\0') {
    if (! extraChars)
      return COptReal();
  }

  return COptReal(real);
}

COptLong
CJString::
parseInt(const std::string &text, const COptInt &base, bool extraChars)
{
  // skip leading space
  int pos = 0;
  int len = text.size();

  while (pos < len) {
    int pos1 = pos;

    ulong c = CUtf8::readNextChar(text, pos1, len);

    if (! CUtf8::isSpace(c))
      break;

    pos = pos1;
  }

  // Fail on empty string
  if (pos >= len)
    return COptLong();

  //---

  long integer = 0;

  if (! base.isValid()) {
    if (text[pos] == '0' && (text[pos + 1] == 'x' || text[pos + 1] == 'X')) {
      pos += 2;

      std::string text1 = text.substr(pos);

      return parseInt(text1, COptInt(16), extraChars);
    }
    else {
      const char *p1 = &text[pos];
      const char *p2;

      errno = 0;

      integer = strtol(p1, (char **) &p2, 10);

      if (errno == ERANGE)
        return COptLong();

      if (p1 == p2)
        return COptLong();

      while (*p2 != 0 && ::isspace(*p2))
        ++p2;

      if (*p2 != '\0') {
        if (! extraChars)
          return COptLong();
      }
    }
  }
  else {
    int sign = 1;

    int base1 = base.getValue();

    if (pos < len) {
      int pos1 = pos;

      ulong c = CUtf8::readNextChar(text, pos1, len);

      if (c == '-') {
        pos = pos1;

        sign = -1;
      }
    }

    int nd = 0;

    while (pos < len) {
      ulong c = CUtf8::readNextChar(text, pos, len);

      if (c > 0x7f || ! CJUtil::isBaseChar(c, base1))
        break;

      integer = integer*base1 + CJUtil::baseCharValue(c, base1);

      ++nd;
    }

    if (nd == 0)
      return COptLong();

    integer *= sign;
  }

  return COptLong(integer);
}

COptBool
CJString::
parseBool(const std::string &text, bool extraChars)
{
  // skip leading space
  int pos = 0;
  int len = text.size();

  while (pos < len) {
    int pos1 = pos;

    ulong c = CUtf8::readNextChar(text, pos1, len);

    if (! CUtf8::isSpace(c))
      break;

    pos = pos1;
  }

  // Fail on empty string
  if (pos >= len)
    return COptBool();

  //---

  const char *p1 = &text[pos];
  const char *p2;

  errno = 0;

  long integer = strtol(p1, (char **) &p2, 10);

  if (errno == ERANGE)
    return COptBool();

  if (p1 == p2)
    return COptBool();

  while (*p2 != 0 && ::isspace(*p2))
    ++p2;

  if (*p2 != '\0') {
    if (! extraChars)
      return COptBool();
  }

  return COptBool(bool(integer));
}

CJValueP
CJString::
indexValue(int ind) const
{
  int i1, i2;

  if (! CUtf8::indexChar(text_, ind, i1, i2))
    return CJValueP();

  return js_->createStringValue(text_.substr(i1, i2 - i1));
}

void
CJString::
setIndexValue(int ind, CJValueP value)
{
  if (ind < 0 || ind >= int(text_.length()))
    return;

  std::string s = value->toString();

  text_ = text_.substr(0, ind - 1) + s + text_.substr(ind + 1);
}

void
CJString::
deleteIndexValue(int)
{
  // false
}

bool
CJString::
hasIndexValue(int ind) const
{
  return (ind >= 0 && ind < int(text_.length()));
}

long
CJString::
length() const
{
  return CUtf8::length(text_);
}

std::string
CJString::
substr(int ind) const
{
  return CUtf8::substr(text_, ind);
}

std::string
CJString::
substr(int ind, int n) const
{
  return CUtf8::substr(text_, ind, n);
}

std::string
CJString::
toString() const
{
  return text_;
}

void
CJString::
print(std::ostream &os) const
{
  if (! isPrimitive())
    os << "[String: '" << encodeText() << "']";
  else
    os << "'" << encodeText() << "'";
}

std::string
CJString::
encodeText() const
{
  static char buffer[16];

  std::string str;

  int  i   = 0;
  uint len = text_.size();

  while (i < int(len)) {
    ulong c = CUtf8::readNextChar(text_, i, len);

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

      str += "\\u" << str2;
#endif
    }
  }

  return str;
}

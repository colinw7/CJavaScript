#include <CJString.h>
#include <CJavaScript.h>
#include <CJUtil.h>
#include <CUtf8.h>
#include <cstring>

CJObjTypeP CJStringType::type_;

CJObjTypeP
CJStringType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJStringType(js));

    js->addObjectType("string", type_);

    js->addTypeObject(CJToken::Type::String, type_);
  }

  return type_;
}

CJStringType::
CJStringType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::String, "string")
{
  // String properties
  //   length, fromCharCode, fromCodePoint, raw
  // Function
  //   arguments, caller, displayName, length, name, prototype
  // Object
  //   __proto__, constructor
  addTypeFunction(js, "fromCharCode");
  addTypeFunction(js, "fromCodePoint");
  addTypeFunction(js, "raw");
  addTypeFunction(js, "toString");

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
  addObjectFunction(js, "toString");
  addObjectFunction(js, "charAt");
  addObjectFunction(js, "concat");
  addObjectFunction(js, "indexOf");
  addObjectFunction(js, "lastIndexOf");
  addObjectFunction(js, "replace");
  addObjectFunction(js, "slice");
  addObjectFunction(js, "split");
  addObjectFunction(js, "substr");
  addObjectFunction(js, "substring");
  addObjectFunction(js, "toLowerCase");
  addObjectFunction(js, "toUpperCase");
}

CJValueP
CJStringType::
execType(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  // values[0] is CJStringFunction

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

  CJString *cstr = values[0]->cast<CJString>();
  assert(cstr);

  //---

  // object functions
  if      (name == "toString") {
    std::string str = cstr->text();

    return js->createStringValue(str);
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
  else if (name == "concat") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string str1 = (values[1] ? values[1]->toString() : std::string());

    std::string str = cstr->text();

    return js->createStringValue(str + str1);
  }
  else if (name == "indexOf") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    CJString *key = 0;

    if (values[1] && values[1]->type() == CJToken::Type::String)
      key = values[1]->cast<CJString>();

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

    CJString *key = 0;

    if (values[1] && values[1]->type() == CJToken::Type::String)
      key = values[1]->cast<CJString>();

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
  else if (name == "slice") {
    if (values.size() != 2 && values.size() != 3) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    long ind1 = (values[1] ? values[1]->toInteger() : 0);

    if (values.size() == 3) {
      long ind2 = (values[2] ? values[2]->toInteger() : 0);

      int len = cstr->length();

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

    CJArrayP array(new CJArray(js));

    for (const auto &s : strs) {
      CJValueP value = js->createStringValue(s);

      array->addValue(value);
    }

    return std::static_pointer_cast<CJValue>(array);
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
  else if (name == "toLowerCase") {
    std::string str = cstr->text();

    for (auto & elem : str)
      elem = tolower(elem);

    return js->createStringValue(str);
  }
  else if (name == "toUpperCase") {
    std::string str = cstr->text();

    for (auto & elem : str)
      elem = toupper(elem);

    return js->createStringValue(str);
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

  return CJValueP(new CJString(js_, text_.substr(i1, i2 - i1)));
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
  if (! isPrimitive())
    return "[String: '" + text_ + "']";
  else
    return text_;
}

void
CJString::
print(std::ostream &os) const
{
  static char buffer[16];

  os << "\'";

  int  i   = 0;
  uint len = text_.size();

  while (i < int(len)) {
    ulong c = CUtf8::readNextChar(text_, i, len);

    if (c <= 0x7f) {
      if (c <= 0x1f) {
        if      (c == '\b')
          os << "\\b";
        else if (c == '\f')
          os << "\\f";
        else if (c == '\n')
          os << "\\n";
        else if (c == '\r')
          os << "\\r";
        else if (c == '\t')
          os << "\\t";
        else if (c == '\v')
          os << "\\v";
        else {
          int i = (c & 0xff);

          ::sprintf(buffer, "%02x", i);

          os << "\\x" << buffer;
        }
      }
      else {
        os << char(c);
      }
    }
    else {
      int len1;

      CUtf8::encode(c, buffer, len1);

      for (int j = 0; j < len1; ++j)
        os << buffer[j];

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

      os << "\\u" << str2;
#endif
    }
  }

  os << "\'";
}

#include <CJString.h>
#include <CJRegExp.h>
#include <CJObjectTypeFunction.h>
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
  addObjectFunction(js_, "anchor"           , type_)->setNumArgs(1);
  addObjectFunction(js_, "charAt"           , type_)->setNumArgs(1);
  addObjectFunction(js_, "charCodeAt"       , type_)->setNumArgs(1);
  addObjectFunction(js_, "codePointAt"      , type_)->setNumArgs(1);
  addObjectFunction(js_, "concat"           , type_)->setNumArgs(1);
  addObjectFunction(js_, "endsWith"         , type_)->setNumArgs(1);
  addObjectFunction(js_, "includes"         , type_)->setNumArgs(1);
  addObjectFunction(js_, "indexOf"          , type_)->setNumArgs(1);
  addObjectFunction(js_, "lastIndexOf"      , type_)->setNumArgs(1);
  addObjectFunction(js_, "link"             , type_)->setNumArgs(1);
  addObjectFunction(js_, "localeCompare"    , type_)->setNumArgs(1);
  addObjectFunction(js_, "match"            , type_)->setNumArgs(1);
  addObjectFunction(js_, "normalize"        , type_)->setNumArgs(1);
  addObjectFunction(js_, "repeat"           , type_)->setNumArgs(1);
  addObjectFunction(js_, "replace"          , type_)->setNumArgs(1);
  addObjectFunction(js_, "search"           , type_)->setNumArgs(1);
  addObjectFunction(js_, "slice"            , type_)->setNumArgs(1);
  addObjectFunction(js_, "split"            , type_)->setNumArgs(1);
  addObjectFunction(js_, "startsWith"       , type_)->setNumArgs(1);
  addObjectFunction(js_, "substr"           , type_)->setNumArgs(2);
  addObjectFunction(js_, "substring"        , type_)->setNumArgs(2);
  addObjectFunction(js_, "toLocaleLowerCase", type_)->setNumArgs(0);
  addObjectFunction(js_, "toLocaleUpperCase", type_)->setNumArgs(0);
  addObjectFunction(js_, "toLowerCase"      , type_)->setNumArgs(0);
  addObjectFunction(js_, "toString"         , type_)->setNumArgs(0);
  addObjectFunction(js_, "toUpperCase"      , type_)->setNumArgs(0);
  addObjectFunction(js_, "trim"             , type_)->setNumArgs(0);
  addObjectFunction(js_, "trimLeft"         , type_)->setNumArgs(0);
  addObjectFunction(js_, "trimRight"        , type_)->setNumArgs(0);
  addObjectFunction(js_, "valueOf"          , type_)->setNumArgs(0);
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
      long code = values[i]->toInteger().getValue(0);

      CUtf8::append(str, code);
    }

    return js->createStringValue(str);
  }
  else if (name == "fromCodePoint") {
    std::string str;

    for (uint i = 1; i < values.size(); ++i) {
      long code = values[i]->toInteger().getValue(0);

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

  CJValueP thisValue = values[0];

  std::string str;
  long        len;

  if (thisValue && thisValue->type() == CJToken::Type::String) {
    CJStringP cstr = CJValue::cast<CJString>(thisValue);

    str = cstr->text();
    len = cstr->length().getValue(0);
  }
  else {
    str = thisValue->toString();
    len = str.size();
  }

  //---

  // object functions
  if      (name == "anchor") {
    return CJValueP();
  }
  else if (name == "charAt") {
    long ind = 0;

    if (values.size() > 1)
      ind = (values[1] ? values[1]->toInteger().getValue(0) : 0);

    std::string res;

    if (ind >= 0 && ind < len) {
      res = CJString::substr(str, len, ind, 1);
    }

    return js->createStringValue(res);
  }
  else if (name == "charCodeAt") {
    long ind = 0;

    if (values.size() > 1)
      ind = (values[1] ? values[1]->toInteger().getValue(0) : 0);

    long c = -1;

    if (ind >= 0 && ind < len) {
      int i1, i2;

      if (! CUtf8::indexChar(str, ind, i1, i2))
        c = -1;

      c = CUtf8::readNextChar(str, i1, len);
    }

    if (c >= 0)
      return js->createNumberValue(c);
    else
      return js->createNumberValue(CJUtil::getNaN());
  }
  else if (name == "codePointAt") {
    return CJValueP();
  }
  else if (name == "concat") {
    std::string str1;

    for (uint i = 1; i < values.size(); ++i) {
      str1 = (values[i] ? values[i]->toString() : std::string());

      str += str1;
    }

    return js->createStringValue(str);
  }
  else if (name == "endsWith") {
    return CJValueP();
  }
  else if (name == "includes") {
    return CJValueP();
  }
  else if (name == "indexOf") {
    // get key
    std::string key;

    if (values.size() > 1)
      key = (values[1] ? values[1]->toString() : std::string());

    // get start
    COptLong start;

    if (values.size() > 2) {
      start = values[2]->toInteger().getValue(0);

      if (start < 0)
        start = 0;
    }

    // index
    long ind = -1;

    if (CUtf8::isAscii(str)) {
      if (start.isValid()) {
        auto p = str.find(key, start.getValue(0));

        if (p == std::string::npos)
          ind = long(-1);
        else
          ind = long(p);
      }
      else {
        auto p = str.find(key);

        if (p == std::string::npos)
          ind = long(-1);
        else
          ind = long(p);
      }
    }
    else {
      int n   = 0;
      int pos = 0;
      int len = str.size();

      while (pos < len) {
        int pos1 = pos;

        (void) CUtf8::readNextChar(str, pos, len);

        if (! start.isValid() || n >= start.getValue(0)) {
          if (strncmp(&key[0], &str[pos1], key.size()) == 0) {
            ind = n;
            break;
          }
        }

        ++n;
      }
    }

    return js->createNumberValue(ind);
  }
  else if (name == "lastIndexOf") {
    // get key
    std::string key;

    if (values.size() > 1)
      key = (values[1] ? values[1]->toString() : std::string());

    // get start
    COptLong start;

    if (values.size() > 2) {
      double rstart = values[2]->toReal().getValue(CJUtil::getNaN());

      if (! CJUtil::isNaN(rstart))
        start = values[2]->toInteger().getValue(CJUtil::maxInteger());
      else
        start = CJUtil::maxInteger();

      if (start < 0)
        start = 0;
    }

    // index
    long ind = -1;

    if (CUtf8::isAscii(str)) {
      if (start.isValid()) {
        std::string str1 = str.substr(0, start.getValue(0) + key.size());

        auto p = str1.rfind(key);

        if (p == std::string::npos)
          ind = long(-1);
        else
          ind = long(p);
      }
      else {
        auto p = str.rfind(key);

        if (p == std::string::npos)
          ind = long(-1);
        else
          ind = long(p);
      }
    }
    else {
      int n   = 0;
      int pos = 0;
      int len = str.size();

      while (pos < len) {
        int pos1 = pos;

        (void) CUtf8::readNextChar(str, pos, len);

        if (! start.isValid() || n <= start.getValue(0)) {
          if (strncmp(&key[0], &str[pos1], key.size()) == 0)
            ind = n;
        }

        ++n;
      }
    }

    return js->createNumberValue(ind);
  }
  else if (name == "link") {
    return CJValueP();
  }
  else if (name == "localeCompare") {
    std::string lhs = str;

    std::string rhs;

    if (values.size() > 1)
      rhs = (values[1] ? values[1]->toString() : "");

    CJValueP locales;

    if (values.size() > 2)
      locales = values[2];

    CJValueP options;

    if (values.size() > 2)
      options = values[2];

    long cmp = strcmp(lhs.c_str(), rhs.c_str());

    long cmp1 = (cmp >= 0 ? (cmp > 0 ? 1 : 0) : -1);

    return js->createNumberValue(cmp1);
  }
  else if (name == "match") {
    CJValueP rvalue;

    if (values.size() > 1)
      rvalue = values[1];

    if (rvalue && rvalue->type() != CJToken::Type::RegExp)
      rvalue = js->createRegExpValue(rvalue->toString());

    if (! rvalue) {
      js->errorMsg("Invalid regexp for " + name);
      return CJValueP();
    }

    CJRegExpP regexp = CJValue::cast<CJRegExp>(rvalue);

    CJRegExp::MatchData data;

    bool b = regexp->find(str, data);

    if (! b)
      return js->createNullValue();

    CJArrayP array = js->createArrayValue();

    if (regexp->isGlobalMatch()) {
      array->setIndexValue(0, js->createStringValue(regexp->text()));

      for (uint i = 0; i < data.subMatches.size(); ++i)
        array->setIndexValue(i + 1, js->createStringValue(data.subMatches[i]));
    }
    else {
      array->setIndexValue(0, js->createStringValue(regexp->text()));

      array->setProperty(js, "index", js->createNumberValue(long(data.range.first)));
      array->setProperty(js, "input", js->createStringValue(str));
    }

    return array;
  }
  else if (name == "normalize") {
    return CJValueP();
  }
  else if (name == "repeat") {
    return CJValueP();
  }
  else if (name == "replace") {
    CJRegExpP   regexp;
    std::string substr;

    if (values.size() > 1 && values[1]) {
      if (values[1]->type() == CJToken::Type::RegExp)
        regexp = CJValue::cast<CJRegExp>(values[1]);
      else
        substr = values[1]->toString();
    }

    CJFunctionBaseP fn;
    std::string     newSubstr;

    if (values.size() > 2 && values[2]) {
      if (values[2]->isFunction())
        fn = CJValue::cast<CJFunctionBase>(values[1]);
      else
        newSubstr = values[2]->toString();
    }

    std::string::size_type start = 0, end = len - 1;

    if (regexp) {
      CJRegExp::MatchData data;

      if (! regexp->find(str, data))
        return thisValue;

      start = data.range.first;
      end   = data.range.second;

      std::string newSubstr1;

      for (uint i = 0; i < newSubstr.size(); ++i) {
        if (newSubstr[i] == '$' && isdigit(newSubstr[i + 1])) {
          int ind = newSubstr[i + 1] - '0';

          if (ind >= 1 && i <= data.subMatches.size())
            newSubstr1 += data.subMatches[ind - 1];

          ++i;
        }
        else
          newSubstr1 += newSubstr[i];
      }

      newSubstr = newSubstr1;
    }
    else {
      start = str.find(substr);

      if (start == std::string::npos)
        return thisValue;

      end = start + substr.length() - 1;
    }

    if (fn) {
      js->errorMsg("Function not supported for " + name);
      return thisValue;
    }

    str.replace(start, end - start + 1, newSubstr);

    return js->createStringValue(str);
  }
  else if (name == "search") {
    CJValueP rvalue;

    if (values.size() > 1)
      rvalue = values[1];

    if (rvalue && rvalue->type() != CJToken::Type::RegExp)
      rvalue = js->createRegExpValue(rvalue->toString());

    if (! rvalue) {
      js->errorMsg("Invalid regexp for " + name);
      return CJValueP();
    }

    CJRegExpP regexp = CJValue::cast<CJRegExp>(rvalue);

    CJRegExp::MatchData data;

    if (! regexp->find(str, data))
      return js->createNumberValue(-1L);

    return js->createNumberValue(long(data.range.first));
  }
  else if (name == "slice") {
    // get start ind
    long ind1 = 0;

    if (values.size() > 1) {
      ind1 = (values[1] ? values[1]->toInteger().getValue(0) : 0);

      if (ind1 < 0)
        ind1 = len + ind1;

      ind1 = CJUtil::clamp(ind1, 0L, len);
    }

    //---

    std::string res;

    if (values.size() > 2) {
      // get end index
      long ind2 = (values[2] ? values[2]->toInteger().getValue(len) : len);

      if (ind2 < 0)
        ind2 = len + ind2;

      ind2 = CJUtil::clamp(ind2, 0L, len);

      //---

      if ((ind1 >= 0 && ind1 < len) && (ind2 >= 0 && ind2 <= len) && ind1 <= ind2) {
        res = CJString::substr(str, len, ind1, ind2 - ind1);
      }
    }
    else {
      if (ind1 >= 0 && ind1 < len) {
        res = CJString::substr(str, len, ind1);
      }
    }

    return js->createStringValue(res);
  }
  else if (name == "split") {
    std::vector<std::string> strs;

    COptString sstr;
    CJRegExpP  regexp;

    if (values.size() > 1) {
      if (values[1] && values[1]->type() == CJToken::Type::RegExp)
        regexp = CJValue::cast<CJRegExp>(values[1]);
      else
        sstr = (values[1] ? values[1]->toString() : std::string());
    }

    COptLong limit;

    if (values.size() > 2) {
      if (! values[2] || js->isUndefinedOrNullValue(values[2]))
        limit = CJUtil::maxInteger();
      else
        limit = values[2]->toInteger().getValue(0);

      if (limit < 0)
        limit.setInvalid();
    }

    if      (! sstr.isValid() && ! regexp) {
      strs.push_back(str);
    }
    else if (regexp) {
      CJRegExp::MatchData data;

      std::string str1 = str;

      while (regexp->find(str1, data)) {
        std::string lhs = str1.substr(0, data.range.first);
        std::string rhs = str1.substr(data.range.second + 1);

        if (! limit.isValid() || long(strs.size()) < limit.getValue(0))
          strs.push_back(lhs);

        str1 = rhs;
      }

      if (! limit.isValid() || long(strs.size()) < limit.getValue(0))
        strs.push_back(str1);
    }
    else if (sstr.isValid() && ! sstr.getValue().empty()) {
      auto p = str.find(sstr.getValue());

      while (p != std::string::npos) {
        if (! limit.isValid() || long(strs.size()) < limit.getValue(0))
          strs.push_back(str.substr(0, p));

        str = str.substr(p + sstr.getValue().size());

        p = str.find(sstr.getValue());
      }

      if (! limit.isValid() || long(strs.size()) < limit.getValue(0))
        strs.push_back(str);
    }
    else {
      int pos = 0;
      int len = str.size();

      while (pos < len) {
        ulong c = CUtf8::readNextChar(str, pos, len);

        std::string str1;

        CUtf8::append(str1, c);

        if (! limit.isValid() || long(strs.size()) < limit.getValue(0))
          strs.push_back(str1);
      }

      if (strs.empty()) {
        if (! limit.isValid() || long(strs.size()) < limit.getValue(0))
          strs.push_back("");
      }
    }

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
    // get start ind
    long start = 0;

    if (values.size() > 1) {
      start = (values[1] ? values[1]->toInteger().getValue(0) : 0);

      if (start < 0)
        start += len;
    }

    //---

    std::string res;

    if (values.size() > 2) {
      // get length
      long len1 = (values[2] ? values[2]->toInteger().getValue(len) : len);

      long end = start + len1;

      end = CJUtil::clamp(end, 0L, len);

      //---

      if ((start >= 0 && start < len) && (end >= 0 && end <= len) && start <= end) {
        res = CJString::substr(str, len, start, end - start);
      }
    }
    else {
      if (start >= 0 && start < len) {
        res = CJString::substr(str, len, start);
      }
    }

    return js->createStringValue(res);
  }
  else if (name == "substring") {
    // get start ind
    long ind1 = 0;

    if (values.size() > 1) {
      ind1 = (values[1] ? values[1]->toInteger().getValue(0) : 0);

      ind1 = CJUtil::clamp(ind1, 0L, len);
    }

    //---

    std::string res;

    if (values.size() > 2) {
      // get end
      long ind2 = (values[2] ? values[2]->toInteger().getValue(len) : len);

      ind2 = CJUtil::clamp(ind2, 0L, len);

      if (ind1 > ind2)
        std::swap(ind1, ind2);

      //---

      if ((ind1 >= 0 && ind1 < len) && (ind2 >= 0 && ind2 <= len) && ind1 <= ind2) {
        res = CJString::substr(str, len, ind1, ind2 - ind1);
      }
    }
    else {
      if (ind1 >= 0 && ind1 < len) {
        res = CJString::substr(str, len, ind1);
      }
    }

    return js->createStringValue(res);
  }
  else if (name == "toLocalLowerCase") {
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
  else if (name == "toLocalUpperCase") {
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
  else if (name == "toLowerCase") {
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
    return js->createStringValue(str);
  }
  else if (name == "toUpperCase") {
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
    std::string str1 = CJString::trim(str, len, true, true);

    return js->createStringValue(str1);
  }
  else if (name == "trimLeft") {
    std::string str1 = CJString::trim(str, len, true, false);

    return js->createStringValue(str1);
  }
  else if (name == "trimRight") {
    std::string str1 = CJString::trim(str, len, false, true);

    return js->createStringValue(str1);
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

CJString::
CJString(const CJString &str) :
 CJObj(str), text_(str.text_), c_(str.c_)
{
}

CJValueP
CJString::
getProperty(CJavaScript *js, const std::string &key) const
{
  if (key == "length")
    return js->createNumberValue(length().getValue(0));

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

COptReal
CJString::
toReal() const
{
  COptReal real = parseFloat(text_);

  return real;
}

COptLong
CJString::
toInteger() const
{
  COptLong integer = parseInt(text_);

  return integer;
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
    return COptReal(CJUtil::getNaN());

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
indexValue(long ind) const
{
  int i1, i2;

  if (! CUtf8::indexChar(text_, ind, i1, i2))
    return CJValueP();

  return js_->createStringValue(text_.substr(i1, i2 - i1));
}

void
CJString::
setIndexValue(long ind, CJValueP value)
{
  if (ind < 0 || ind >= long(text_.length()))
    return;

  std::string s = value->toString();

  text_ = text_.substr(0, ind - 1) + s + text_.substr(ind + 1);
}

void
CJString::
deleteIndexValue(long)
{
  // false
}

bool
CJString::
hasIndexValue(long ind) const
{
  return (ind >= 0 && ind < long(text_.length()));
}

COptLong
CJString::
length() const
{
  return COptLong(CUtf8::length(text_));
}

std::string
CJString::
substr(long ind) const
{
  return CUtf8::substr(text_, ind);
}

std::string
CJString::
substr(long ind, long n) const
{
  return substr(text_, ind, n);
}

std::string
CJString::
substr(const std::string &text, long /*len*/, long ind)
{
  return CUtf8::substr(text, ind);
}

std::string
CJString::
substr(const std::string &text, long /*len*/, long ind, long n)
{
  return CUtf8::substr(text, ind, n);
}

std::string
CJString::
trim(bool left, bool right) const
{
  std::string str = this->text();

  return trim(str, str.size(), left, right);
}

std::string
CJString::
trim(const std::string &str, long len, bool left, bool right)
{
  int pos = 0;

  //---

  // find last start space and first end space
  bool start = true;
  bool end   = true;

  int startPos = 0;
  int endPos   = len;

  while (pos < len) {
    int pos1 = pos;

    ulong c = CUtf8::readNextChar(str, pos, len);

    if      (CUtf8::isSpace(c)) {
      if      (start)
        startPos = pos1;
      else if (end) {
        endPos = pos1;
        end    = false;
      }
    }
    else {
      if (start) {
        startPos = pos1;

        start = false;
        end   = true;
      }
      else {
        end = true;
      }
    }
  }

  //---

  // if still looking for start we have all spaces
  if (start)
    return "";

  //---

  // adjust start and end depending on whether found ot trimming at start/end
  if (! left || start)
    startPos = 0;

  if (! right || end)
    endPos = len;

  //---

  // add characters between start/end
  std::string str1;

  pos = 0;

  while (pos < len) {
    int pos1 = pos;

    ulong c = CUtf8::readNextChar(str, pos, len);

    if (pos1 < startPos || pos1 >= endPos)
      continue;

    CUtf8::append(str1, c);
  }

  return str1;
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

  while (i < long(len)) {
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

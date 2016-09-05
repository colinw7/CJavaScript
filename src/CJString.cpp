#include <CJString.h>
#include <CJavaScript.h>
#include <CJUtil.h>
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
  addTypeFunction(js, "fromCharCode");
  addTypeFunction(js, "fromCodePoint");
  addTypeFunction(js, "raw");

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
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJString *cstr = values[0]->cast<CJString>();
  assert(cstr);

  std::string str = cstr->text();

  //---

  // type functions
  if      (name == "fromCharCode") {
    return CJValueP();
  }
  else if (name == "fromCodePoint") {
    return CJValueP();
  }
  else if (name == "raw") {
    return CJValueP();
  }
  // object functions
  else if (name == "charAt") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    long ind = (values[1] ? values[1]->toInteger() : 0);

    if (ind >= 0 && ind < long(str.size())) {
      return js->createStringValue(str.substr(ind, 1));
    }
  }
  else if (name == "concat") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string str1 = (values[1] ? values[1]->toString() : std::string());

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

    if (key) {
      auto p = str.find(key->text());

      if (p == std::string::npos)
        return js->createNumberValue(long(-1));

      return js->createNumberValue(long(p));
    }
  }
  else if (name == "lastIndexOf") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    CJString *key = 0;

    if (values[1] && values[1]->type() == CJToken::Type::String)
      key = values[1]->cast<CJString>();

    if (key) {
      auto p = str.rfind(key->text());

      if (p == std::string::npos)
        return js->createNumberValue(long(-1));

      return js->createNumberValue(long(p));
    }
  }
  else if (name == "replace") {
    if (values.size() != 3) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string str1 = (values[1] ? values[1]->toString() : std::string());
    std::string str2 = (values[2] ? values[2]->toString() : std::string());

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

      if ((ind1 >= 0 && ind1 < long(str.size())) &&
          (ind2 >= 0 && ind2 < long(str.size())) &&
          ind1 <= ind2) {
        return js->createStringValue(str.substr(ind1, ind2 - ind1));
      }
    }
    else {
      if (ind1 >= 0 && ind1 < long(str.size())) {
        return js->createStringValue(str.substr(ind1));
      }
    }
  }
  else if (name == "split") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

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
      long len = (values[2] ? values[2]->toInteger() : 0);

      long end = start + len;

      if ((start >= 0 && start < long(str.size())) &&
          (end   >= 0 && end   < long(str.size())) &&
          start <= end) {
        return js->createStringValue(str.substr(start, end - start));
      }
    }
    else {
      if (start >= 0 && start < long(str.size())) {
        return js->createStringValue(str.substr(start));
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

      if ((ind1 >= 0 && ind1 < long(str.size())) &&
          (ind2 >= 0 && ind2 < long(str.size())) &&
          ind1 <= ind2) {
        return js->createStringValue(str.substr(ind1, ind2 - ind1));
      }
    }
    else {
      if (ind1 >= 0 && ind1 < long(str.size())) {
        return js->createStringValue(str.substr(ind1));
      }
    }
  }
  else if (name == "toLowerCase") {
    for (auto & elem : str)
      elem = tolower(elem);

    return js->createStringValue(str);
  }
  else if (name == "toUpperCase") {
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
  const char *c_str = text.c_str();

  // skip leading space
  int i = 0;

  while (c_str[i] != 0 && ::isspace(c_str[i]))
    ++i;

  // Fail on empty string
  if (c_str[i] == '\0')
    return COptReal();

  const char *p1 = &c_str[i];

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
parseInt(const std::string &text, bool extraChars)
{
  const char *c_str = text.c_str();

  // skip leading space
  int i = 0;

  while (c_str[i] != 0 && ::isspace(c_str[i]))
    ++i;

  // Fail on empty string
  if (c_str[i] == '\0')
    return COptLong();

  const char *p1 = &c_str[i];
  const char *p2;

  errno = 0;

  long integer = strtol(p1, (char **) &p2, 10);

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

  return COptLong(integer);
}

COptBool
CJString::
parseBool(const std::string &text, bool extraChars)
{
  const char *c_str = text.c_str();

  // skip leading space
  int i = 0;

  while (c_str[i] != 0 && ::isspace(c_str[i]))
    ++i;

  // Fail on empty string
  if (c_str[i] == '\0')
    return COptBool();

  const char *p1 = &c_str[i];
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
  if (ind < 0 || ind >= int(text_.length()))
    return CJValueP();

  return CJValueP(new CJString(js_, text_.substr(ind, 1)));
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

bool
CJString::
hasIndexValue(int ind) const
{
  return (ind >= 0 && ind < int(text_.length()));
}

void
CJString::
print(std::ostream &os) const
{
  os << "\'" << text_ << "\'";
}

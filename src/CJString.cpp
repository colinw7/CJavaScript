#include <CJString.h>
#include <CJavaScript.h>
#include <CJUtil.h>

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

    CJString *key = (values[1] ? values[1]->cast<CJString>() : 0);

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

    CJString *key = (values[1] ? values[1]->cast<CJString>() : 0);

    if (key) {
      auto p = str.rfind(key->text());

      if (p == std::string::npos)
        return js->createNumberValue(long(-1));

      return js->createNumberValue(long(p));
    }
  }
#if 0
  else if (name == "length") {
    long len = cstr->length();

    return js->createNumberValue(len);
  }
#endif
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

    auto p = str.find(sstr);

    while (p != std::string::npos) {
      strs.push_back(str.substr(0, p));

      str = str.substr(p + sstr.size());

      p = str.find(sstr);
    }

    strs.push_back(str);

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
    for (size_t i = 0; i < str.size(); ++i)
      str[i] = tolower(str[i]);

    return js->createStringValue(str);
  }
  else if (name == "toUpperCase") {
    for (size_t i = 0; i < str.size(); ++i)
      str[i] = toupper(str[i]);

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
 CJObj(CJStringType::instance(js)), js_(js), text_(text), c_(c)
{
  addPseudoProperty("length");
}

CJValueP
CJString::
getProperty(const std::string &key) const
{
  if (key == "length")
    return js_->createNumberValue(length());

  return CJObj::getProperty(key);
}

void
CJString::
setProperty(const std::string &key, CJValueP value)
{
  if (key == "length") {
    // TODO:
    //return CJValueP();
  }

  CJObj::setProperty(key, value);
}

double
CJString::
toReal() const
{
  return parseFloat(text_);
}

long
CJString::
toInteger() const
{
  return parseInt(text_);
}

double
CJString::
parseFloat(const std::string &text)
{
  const char *c_str = text.c_str();

  // skip leading space
  int i = 0;

  while (c_str[i] != 0 && ::isspace(c_str[i]))
    ++i;

  if (c_str[i] == '\0') {
    // Empty string is 0
    return 0;
  }

  const char *p;

  errno = 0;

  double real = strtod(&c_str[i], (char **) &p);

  if (errno == ERANGE) {
    return CJUtil::getNaN();
  }

  while (*p != 0 && ::isspace(*p))
    ++p;

  if (*p != '\0') {
    return CJUtil::getNaN();
  }

  return real;
}

long
CJString::
parseInt(const std::string &text)
{
  const char *c_str = text.c_str();

  // skip leading space
  int i = 0;

  while (c_str[i] != 0 && ::isspace(c_str[i]))
    ++i;

  if (c_str[i] == '\0') {
    // Empty string is 0
    return 0;
  }

  const char *p;

  errno = 0;

  long integer = strtol(&c_str[i], (char **) &p, 10);

  if (errno == ERANGE) {
    // TODO: error
    return 0;
  }

  while (*p != 0 && ::isspace(*p))
    ++p;

  if (*p != '\0') {
    // TODO: error
    return integer;
  }

  return integer;
}

bool
CJString::
toBoolean() const
{
  const char *c_str = text_.c_str();

  // skip leading space
  int i = 0;

  while (c_str[i] != 0 && ::isspace(c_str[i]))
    ++i;

  if (c_str[i] == '\0') {
    // Empty string is false
    return false;
  }

  const char *p;

  errno = 0;

  long integer = strtol(&c_str[i], (char **) &p, 10);

  if (errno == ERANGE) {
    // TODO: error
    return true;
  }

  while (*p != 0 && ::isspace(*p))
    ++p;

  if (*p != '\0') {
    // TODO: error
    return true;
  }

  return integer;
}

CJValueP
CJString::
indexValue(int i) const
{
  if (i < 0 || i >= int(text_.length()))
    return CJValueP();

  return CJValueP(new CJString(js_, text_.substr(i, 1)));
}

void
CJString::
setIndexValue(int i, CJValueP value)
{
  if (i < 0 || i >= int(text_.length()))
    return;

  std::string s = value->toString();

  text_ = text_.substr(0, i - 1) + s + text_.substr(i + 1);
}

void
CJString::
print(std::ostream &os) const
{
  os << "\'" << text_ << "\'";
}

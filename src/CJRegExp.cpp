#include <CJRegExp.h>
#include <CJavaScript.h>
#include <CJUtil.h>

CJObjTypeP CJRegExpType::type_;

CJObjTypeP
CJRegExpType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJRegExpType(js));

    js->addObjectType("regexp", type_);

    js->addTypeObject(CJToken::Type::RegExp, type_);
  }

  return type_;
}

CJRegExpType::
CJRegExpType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::RegExp, "regexp")
{
  addTypeFunction(js, "toString");

  addObjectFunction(js, "compile");
  addObjectFunction(js, "exec");
  addObjectFunction(js, "test");
  addObjectFunction(js, "toSource");
  addObjectFunction(js, "toString");
}

CJValueP
CJRegExpType::
execType(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  // values[0] is CJRegExpFunction

  //---

  // type functions
  if (name == "toString") {
    return js->createStringValue("function RegExp() { }");
  }
  else {
    js->errorMsg("Invalid regexp type function " + name);
  }

  return CJValueP();
}

CJValueP
CJRegExpType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJRegExp *regexp = values[0]->cast<CJRegExp>();
  assert(regexp);

  //---

  // object functions
  if      (name == "compile") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string str = (values[1] ? values[1]->toString() : std::string());

    return js->createRegExpValue(str);
  }
  else if (name == "exec") {
    return CJValueP();
  }
  else if (name == "test") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string str = (values[1] ? values[1]->toString() : std::string());

    bool b = regexp->find(str);

    return js->createBoolValue(b);
  }
  else {
    js->errorMsg("Invalid string function " + name);
  }

  return CJValueP();
}

//------

CJRegExp::
CJRegExp(CJavaScript *js, const std::string &text, const std::string &flags) :
 CJObj(js, CJRegExpType::instance(js)), text_(text), flags_(flags), regexp_(text_)
{
  updateFlags();
}

void
CJRegExp::
setText(const std::string &str)
{
  text_ = str;

  regexp_ = CRegExp(text_);

  updateFlags();
}

void
CJRegExp::
updateFlags()
{
  for (auto & elem : flags_) {
    if      (elem == 'g') {
    }
    else if (elem == 'i') {
      regexp_.setCaseSensitive(true);
    }
    else if (elem == 'm') {
    }
    else if (elem == 'u') {
    }
    else if (elem == 'y') {
    }
  }
}

CJValueP
CJRegExp::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJObj::getProperty(js, key);
}

void
CJRegExp::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJObj::setProperty(js, key, value);
}

bool
CJRegExp::
find(const std::string &str) const
{
  return regexp_.find(str);
}

void
CJRegExp::
print(std::ostream &os) const
{
  os << "/" << text_ << "/";
}

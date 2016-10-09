#include <CJRegExp.h>
#include <CJavaScript.h>
#include <CJUtil.h>

CJRegExpTypeP CJRegExpType::type_;

CJRegExpTypeP
CJRegExpType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJRegExpTypeP(new CJRegExpType(js));

    type_->init();

    js->addObjectType(type_->name(), type_);

    js->addTypeObject(CJToken::Type::RegExp, type_);
  }

  return type_;
}

CJRegExpType::
CJRegExpType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::RegExp, "Regexp")
{
}

void
CJRegExpType::
init()
{
  addTypeFunction(js_, "toString");

  addObjectFunction(js_, "compile" , type_);
  addObjectFunction(js_, "exec"    , type_);
  addObjectFunction(js_, "test"    , type_);
  addObjectFunction(js_, "toSource", type_);
  addObjectFunction(js_, "toString", type_);
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

  CJRegExpP regexp = CJValue::cast<CJRegExp>(values[0]);
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

    CJRegExp::MatchData data;

    bool b = regexp->find(str, data);

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
  regexp_.setExtended(true);

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
find(const std::string &str, MatchData &data) const
{
  if (! regexp_.find(str))
    return false;

  int start, end;

  if (regexp_.getMatchRange(&start, &end))
    data.range = MatchData::Range(start, end);

  int n = regexp_.getNumSubMatches();

  for (int i = 0; i < n; ++i) {
    int start, end;

    if (! regexp_.getSubMatchRange(i, &start, &end))
      continue;

    data.subRanges.push_back(MatchData::Range(start, end));

    data.subMatches.push_back(regexp_.getSubMatchString(i));
  }

  return true;
}

std::string
CJRegExp::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJRegExp::
print(std::ostream &os) const
{
  os << "/" << text_ << "/";
}

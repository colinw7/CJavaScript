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
    std::string str;

    if (values.size() >= 2)
      str = (values[1] ? values[1]->toString() : std::string());

    return js->createRegExpValue(str);
  }
  else if (name == "exec") {
    if (values.size() < 2)
      return js->createNullValue();

    std::string str = (values[1] ? values[1]->toString() : std::string());

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

    regexp->setProperty(js, "lastIndex", js->createNumberValue(long(data.range.second)));

    return array;
  }
  else if (name == "test") {
    if (values.size() < 2)
      js->createBoolValue(false);

    std::string str = (values[1] ? values[1]->toString() : std::string());

    CJRegExp::MatchData data;

    bool b = regexp->find(str, data);

    return js->createBoolValue(b);
  }
  else {
    js->errorMsg("Invalid regexp function " + name);
  }

  return CJValueP();
}

//------

CJRegExp::
CJRegExp(CJavaScript *js, const std::string &text) :
 CJObj(js, CJRegExpType::instance(js)), text_(text), regexp_(text_)
{
  regexp_.setExtended(true);

  addPseudoProperty("lastIndex");
  addPseudoProperty("ignoreCase");
  addPseudoProperty("global");
  addPseudoProperty("multiline");
  addPseudoProperty("source");
}

CJRegExp::
CJRegExp(const CJRegExp &regexp) :
 CJObj(regexp), text_(regexp.text_), global_(regexp.global_), multiLine_(regexp.multiLine_),
 lastIndex_(regexp.lastIndex_), regexp_(regexp.regexp_)
{
}

void
CJRegExp::
setText(const std::string &str)
{
  text_ = str;

  regexp_ = CRegExp(text_);

  regexp_.setExtended(true);
}

CJValueP
CJRegExp::
getProperty(CJavaScript *js, const std::string &key) const
{
  if      (key == "lastIndex")
    return js->createNumberValue(lastIndex());
  else if (key == "ignoreCase")
    return js->createBoolValue(isIgnoreCase());
  else if (key == "global")
    return js->createBoolValue(isGlobalMatch());
  else if (key == "multiline")
    return js->createBoolValue(isMultiLine());
  else if (key == "source")
    return js->createStringValue(text());
  else
    return CJObj::getProperty(js, key);
}

void
CJRegExp::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  if      (key == "lastIndex")
    setLastIndex(value->toInteger().getValue(0));
  else if (key == "ignoreCase")
    setIgnoreCase(value->toBoolean());
  else if (key == "global")
    setGlobalMatch(value->toBoolean());
  else if (key == "multiline")
    setMultiLine(value->toBoolean());
  else if (key == "source")
    setText(value->toString());
  else
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

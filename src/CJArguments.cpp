#include <CJArguments.h>
#include <CJavaScript.h>
#include <algorithm>

CJObjTypeP CJArgumentsType::type_;

CJObjTypeP
CJArgumentsType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJArgumentsType(js));

    js->addObjectType("arguments", type_);

    js->addTypeObject(CJToken::Type::Arguments, type_);
  }

  return type_;
}

CJArgumentsType::
CJArgumentsType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Arguments, "arguments")
{
}

CJValueP
CJArgumentsType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  //CJArguments *arguments = values[0]->cast<CJArguments>();
  //assert(arguments);

  return CJValueP();
}

//------

CJArguments::
CJArguments(CJavaScript *js, const Values &values) :
 CJObj(js, CJArgumentsType::instance(js)), values_(values)
{
  addPseudoProperty("length");
}

CJArguments::
CJArguments(const CJArguments &args) :
 CJObj(args), values_(args.values_), callee_(args.callee_)
{
}

void
CJArguments::
addValue(CJValueP value)
{
  values_.push_back(value);
}

CJValueP
CJArguments::
indexValue(long ind) const
{
  if (ind < 0 || ind >= long(values_.size()))
    return CJValueP();

  return values_[ind];
}

void
CJArguments::
setIndexValue(long ind, CJValueP value)
{
  if (ind < 0)
    return;

  while (ind >= long(values_.size()))
    values_.push_back(CJValueP());

  values_[ind] = value;
}

void
CJArguments::
deleteIndexValue(long /*ind*/)
{
}

bool
CJArguments::
hasIndexValue(long ind) const
{
  return (ind >= 0 && ind < long(values_.size()));
}

CJValueP
CJArguments::
getProperty(CJavaScript *js, const std::string &key) const
{
  if      (key == "length")
    return js->createNumberValue(length().getValue(0));
  else if (key == "callee")
    return callee();
  else if (key == "caller")
    return js->createUndefinedValue();

  return CJObj::getProperty(js, key);
}

void
CJArguments::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  if (key == "length") {
    // TODO:
    //return CJValueP();
  }

  CJObj::setProperty(js, key, value);
}

std::string
CJArguments::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJArguments::
print(std::ostream &os) const
{
  os << "[";

  long i = 0;

  for (auto &v : values_) {
    if (i > 0)
      os << ",";

    os << " ";

    if (v)
      os << *v;

    ++i;
  }

  if (i > 0)
    os << " ";

  os << "]";
}

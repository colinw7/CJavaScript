#include <CJFunction.h>
#include <CJArguments.h>
#include <CJavaScript.h>

CJObjTypeP CJFunctionType::type_;

CJObjTypeP
CJFunctionType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJFunctionType(js));

    js->addObjectType("function", type_);

    js->addTypeObject(CJToken::Type::Function, type_);
  }

  return type_;
}

CJFunctionType::
CJFunctionType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Function, "function")
{
  addTypeFunction(js, "toString");

  addObjectFunction(js, "toString");
}

CJValueP
CJFunctionType::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJObjType::getProperty(js, key);
}

CJValueP
CJFunctionType::
execType(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  // values[0] is CJFunctionFunction

  //---

  if (name == "toString") {
    return js->createStringValue("[object Function]");
  }
  else
    return CJValueP();
}

CJValueP
CJFunctionType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJFunction *fn = values[0]->cast<CJFunction>();
  assert(fn);

  //---

  // object functions
  if (name == "toString") {
    return js->createStringValue(fn->toString());
  }
  else
    return CJValueP();
}

//------

CJFunction::
CJFunction(CJavaScript *js, const std::string &name, const Args &args, CJExecBlockP block) :
 CJFunctionBase(js, CJFunctionType::instance(js), name, CJFunctionBase::Type::User),
 args_(args), block_(block)
{
}

void
CJFunction::
setScope(CJavaScript *js, CJDictionaryP parentScope)
{
  scope_ = CJDictionaryP(new CJDictionary(js, name_));

  if (parentScope)
    scope_->setParent(parentScope);

//js->printScopeChain(scope_, "proc " + name_);
}

CJValueP
CJFunction::
getScopeProperty(const std::string &name) const
{
  if (scope_)
    return scope_->getProperty(js_, name);
  else
    return CJValueP();
}

void
CJFunction::
setScopeProperty(const std::string &name, CJValueP value)
{
  if (scope_)
    scope_->setProperty(js_, name, value);
}

std::string
CJFunction::
toString() const
{
  std::ostringstream ss;

  ss << "function " << name() << "(";

  int i = 0;

  for (const auto &arg : args_) {
    if (i > 0)
      ss << ", ";

    ss << arg;

    ++i;
  }

  ss << ")";

  if (block_) {
    ss << " " << *block_;
  }

  return ss.str();
}

CJValueP
CJFunction::
exec(CJavaScript *js, const Values &values)
{
  // set arguments from values
  int nv = values.size();
  int na = args_ .size();

  // TODO: values[0]->this ?
  //if (nv > 0)
  //  setScopeProperty("this", values[0]);

  CJArgumentsP arguments(new CJArguments(js));

  setScopeProperty("arguments", arguments);

  int nva = std::max(nv - 1, na);

  for (int i = 0; i < nva; ++i) {
    CJValueP value;

    if (i < nv - 1)
      value = values[i + 1];
    else
      value = js->createUndefinedValue();

    if (i < na)
      setScopeProperty(args_[i], value);

    arguments->addValue(value);
  }

  //--

  CJFunctionP fn = std::static_pointer_cast<CJFunction>(shared_from_this());

  arguments->setCallee(fn);

  //--

  // call function
  js->startFunctionScope(fn);

  if (block_) {
    js->startBlock(block_);

    block_->exec(js);

    js->endBlock();
  }

  js->endFunctionScope();

  //--

  // set return value
  CJValueP retVal;

  if (block_)
    retVal = block_->getRetVal();

  if (! retVal)
    retVal = js->createUndefinedValue();

  return retVal;
}

void
CJFunction::
print(std::ostream &os) const
{
  os << toString();
}

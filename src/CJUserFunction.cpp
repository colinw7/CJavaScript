#include <CJUserFunction.h>
#include <CJArguments.h>
#include <CJavaScript.h>

CJUserFunction::
CJUserFunction(CJavaScript *js, const std::string &name, const Args &args, CJExecBlockP block) :
 CJFunction(js, name, CJFunction::Type::User), args_(args), block_(block)
{
}

void
CJUserFunction::
setScope(CJavaScript *js, CJDictionaryP parentScope)
{
  scope_ = CJDictionaryP(new CJDictionary(js, name_));

  if (parentScope)
    scope_->setParent(parentScope);

//js->printScopeChain(scope_, "proc " + name_);
}

CJValueP
CJUserFunction::
getScopeProperty(const std::string &name) const
{
  return scope_->getProperty(js_, name);
}

void
CJUserFunction::
setScopeProperty(const std::string &name, CJValueP value)
{
  scope_->setProperty(js_, name, value);
}

CJValueP
CJUserFunction::
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

  CJUserFunctionP fn = std::static_pointer_cast<CJUserFunction>(shared_from_this());

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
CJUserFunction::
print(std::ostream &os) const
{
  os << "function " << name() << "(";

  int i = 0;

  for (const auto &arg : args_) {
    if (i > 0)
      os << ", ";

    os << arg;

    ++i;
  }

  os << ")";

  if (block_) {
    os << " " << *block_;
  }
}

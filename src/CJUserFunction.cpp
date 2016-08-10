#include <CJUserFunction.h>
#include <CJavaScript.h>

CJUserFunction::
CJUserFunction(CJavaScript *js, const std::string &name, const Args &args, CJExecBlockP block) :
 CJFunction(js, name, CJFunction::Type::User), args_(args), block_(block)
{
}

void
CJUserFunction::
setScope(CJavaScript *js, const CJavaScript::UserFunctions &userFunctions)
{
  scope_ = CJDictionaryP(new CJDictionary(js, name_));

  if (! userFunctions.empty())
    scope_->setParent(userFunctions.back()->scope());

//js->printScopeChain(scope_, "proc " + name_);
}

void
CJUserFunction::
setProperty(const std::string &name, CJValueP value)
{
  scope_->setProperty(name, value);
}

CJValueP
CJUserFunction::
exec(CJavaScript *js, const Values &values)
{
  int nv = values.size();
  int na = args_ .size();

  if (nv > 0)
    scope_->setProperty("this", values[0]);

  for (int i = 1; i < nv; ++i) {
    int j = i - 1;

    if (j < na)
      scope_->setProperty(args_[j], values[i]);
  }

  CJUserFunctionP fn = std::static_pointer_cast<CJUserFunction>(shared_from_this());

  js->startFunctionScope(fn);

  js->startBlock(block_);

  block_->exec(js);

  js->endBlock();

  js->endFunctionScope();

  return block_->getRetVal();
}

void
CJUserFunction::
print(std::ostream &os) const
{
  if (block_) {
    os << "function " << name() << "(";

    int i = 0;

    for (const auto &arg : args_) {
      if (i > 0)
        os << ", ";

      os << arg;

      ++i;
    }

    os << ") " << *block_;
  }
}

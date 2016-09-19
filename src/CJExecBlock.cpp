#include <CJExecBlock.h>
#include <CJavaScript.h>

CJExecBlock::
CJExecBlock(Type type) :
 CJToken(CJToken::Type::Block), type_(type)
{
}

void
CJExecBlock::
interp(CJavaScript *js)
{
  CJExecData execData;

  execData.setBlock(true);

  execData.initExec(tokens_);

  js->interp(execData);

  etokens_ = execData.etokens();
}

CJValueP
CJExecBlock::
exec(CJavaScript *js)
{
  CJDictionaryP scope = js->currentScope();

  CJValueP value;

  for (auto &etoken : etokens_) {
    if      (etoken->type() == CJToken::Type::FunctionBase) {
      CJFunctionBaseP fn = std::static_pointer_cast<CJFunctionBase>(etoken);

      scope->setProperty(js, fn->name(), std::static_pointer_cast<CJValue>(fn));
    }
    else if (etoken->isValue()) {
      CJValueP value1 = std::static_pointer_cast<CJValue>(etoken);

      value = value1;
    }
    else
      value = etoken->exec(js);

    CJExecBlockP cblock = js->getCurrentBlock();

    if (cblock &&
        (cblock->isBreakFlag() || cblock->isContinueFlag() || cblock->isReturnFlag()))
      break;

    if (hasError())
      break;
  }

  return value;
}

void
CJExecBlock::
print(std::ostream &os) const
{
  os << "{" << std::endl;

  if (! etokens_.empty()) {
    for (auto &t : etokens_)
      os << " " << *t;
  }
  else {
    for (auto &t : tokens_)
      os << " " << *t;
  }

  os << std::endl << "}";
}


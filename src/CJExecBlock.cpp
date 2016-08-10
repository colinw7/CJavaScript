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
  CJavaScript::ExecData execData;

  execData.setBlock(true);

  execData.initExec(tokens_);

  js->interp(execData);

  etokens_ = execData.etokens();
}

CJValueP
CJExecBlock::
exec(CJavaScript *js)
{
  CJValueP value;

  for (auto &etoken : etokens_) {
    if (etoken->isValue()) {
      CJValueP value1 = std::static_pointer_cast<CJValue>(etoken);

      value = value1;
    }
    else
      value = etoken->exec(js);

    if (isBreakFlag() || isContinueFlag() || isReturnFlag())
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


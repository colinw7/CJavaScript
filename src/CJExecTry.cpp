#include <CJExecTry.h>
#include <CJavaScript.h>

CJExecTry::
CJExecTry() :
 CJToken(CJToken::Type::Try)
{
}

CJValueP
CJExecTry::
exec(CJavaScript *js)
{
  if (! tryBlock_)
    return CJValueP();

  js->startBlock(tryBlock_);

  CJValueP value = tryBlock_->exec(js);

  js->endBlock();

  return value;
}

void
CJExecTry::
print(std::ostream &os) const
{
  if (tryBlock_ && catchIdentifiers_ && catchBlock_) {
    os << "try " << *tryBlock_ << " catch (" << *catchIdentifiers_ << ") " << *catchBlock_;
  }
}


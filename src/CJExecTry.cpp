#include <CJExecTry.h>
#include <CJExecIdentifiers.h>
#include <CJError.h>
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

  if (tryBlock_->hasError()) {
    if (! catchBlock_)
      return CJValueP();

    if (catchIdentifiers_) {
      CJLValueP evar = js->lookupProperty(catchIdentifiers_->identifiers(), /*create*/true);

      CJValueP evalue = std::static_pointer_cast<CJValue>(tryBlock_->error());

      evar->setValue(evalue);
    }

    js->startBlock(catchBlock_);

    value = catchBlock_->exec(js);

    js->endBlock();
  }

  return value;
}

void
CJExecTry::
print(std::ostream &os) const
{
  os << "try";

  if (tryBlock_)
    os << " " << *tryBlock_;

  if (catchIdentifiers_ || catchBlock_) {
    os << " catch";

    if (catchIdentifiers_)
      os << " (" << *catchIdentifiers_ << ")";

    if (catchBlock_)
      os << " " << *catchBlock_;
  }

  if (finallyBlock_) {
    os << "finally " << *finallyBlock_;
  }
}

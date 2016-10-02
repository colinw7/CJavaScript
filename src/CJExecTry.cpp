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
  CJValueP value;

  // try { block }
  if (tryBlock_) {
    js->startBlock(tryBlock_);

    value = tryBlock_->exec(js);

    js->endBlock();
  }

  // execute catch block if error and defined
  if (tryBlock_->hasError() && catchBlock_) {
    if (catchIdentifiers_) {
      CJLValueP evar = js->lookupProperty(catchIdentifiers_->identifiers(), /*create*/true);

      CJErrorBaseP error = tryBlock_->error();

      CJValueP evalue;

      if (error->type() == CJToken::Type::Error)
        evalue = CJValue::cast<CJError>(error)->value();
      else
        evalue = error;

      evar->setValue(evalue);
    }

    js->startBlock(catchBlock_);

    value = catchBlock_->exec(js);

    js->endBlock();
  }

  // finally block always executed
  if (finallyBlock_) {
    js->startBlock(finallyBlock_);

    value = finallyBlock_->exec(js);

    js->endBlock();
  }

  return value;
}

std::string
CJExecTry::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
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

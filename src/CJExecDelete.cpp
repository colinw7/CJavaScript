#include <CJExecDelete.h>
#include <CJavaScript.h>

CJExecDelete::
CJExecDelete() :
 CJToken(CJToken::Type::Return)
{
}

CJValueP
CJExecDelete::
exec(CJavaScript *js)
{
  CJValueP value;

  js->deleteProperty(identifiers_->identifiers());

  return value;
}

void
CJExecDelete::
print(std::ostream &os) const
{
  if (identifiers_)
    os << "delete " << *identifiers_;
}

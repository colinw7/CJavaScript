#include <CJExecDelete.h>
#include <CJExecIdentifiers.h>
#include <CJavaScript.h>

CJExecDelete::
CJExecDelete() :
 CJToken(CJToken::Type::ExecDelete)
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
  os << "delete";

  if (identifiers_)
    os << " " << *identifiers_;

  os << ";";
}

#include <CJExecVoid.h>
#include <CJExecIdentifiers.h>
#include <CJavaScript.h>

CJExecVoid::
CJExecVoid() :
 CJToken(CJToken::Type::ExecVoid)
{
}

CJValueP
CJExecVoid::
exec(CJavaScript *)
{
  CJValueP value;

  return value;
}

void
CJExecVoid::
print(std::ostream &os) const
{
  os << "void ";

  if (identifiers_)
    os << " " << *identifiers_;

  os << ";";
}

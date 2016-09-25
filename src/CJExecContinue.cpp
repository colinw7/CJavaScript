#include <CJExecContinue.h>
#include <CJExecIdentifiers.h>
#include <CJavaScript.h>
#include <iostream>

CJExecContinue::
CJExecContinue() :
 CJToken(CJToken::Type::ExecContinue)
{
}

CJValueP
CJExecContinue::
exec(CJavaScript *js)
{
  CJExecBlockP block = js->getCurrentBlock();

  if (block)
    block->setContinueFlag(true);

  return CJValueP();
}

void
CJExecContinue::
print(std::ostream &os) const
{
  os << "continue";

  if (identifiers_)
    os << " " << *identifiers_;
}

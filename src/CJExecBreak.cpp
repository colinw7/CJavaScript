#include <CJExecBreak.h>
#include <CJExecIdentifiers.h>
#include <CJavaScript.h>
#include <iostream>

CJExecBreak::
CJExecBreak() :
 CJToken(CJToken::Type::ExecBreak)
{
}

CJValueP
CJExecBreak::
exec(CJavaScript *js)
{
  CJExecBlockP block = js->getCurrentBlock();

  if (block)
    block->setBreakFlag(true);

  return CJValueP();
}

void
CJExecBreak::
print(std::ostream &os) const
{
  os << "break";

  if (identifiers_)
    os << " " << *identifiers_;

  os << ";";
}

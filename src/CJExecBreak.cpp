#include <CJExecBreak.h>
#include <CJavaScript.h>

CJValueP
CJExecBreak::
exec(CJavaScript *js)
{
  CJExecBlockP block = js->getCurrentBlock();

  if (block)
    block->setBreakFlag(true);

  return CJValueP();
}

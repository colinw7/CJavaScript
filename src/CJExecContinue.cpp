#include <CJExecContinue.h>
#include <CJavaScript.h>

CJValueP
CJExecContinue::
exec(CJavaScript *js)
{
  CJExecBlockP block = js->getCurrentBlock();

  if (block)
    block->setContinueFlag(true);

  return CJValueP();
}

#include <CJRandFunction.h>
#include <CJavaScript.h>

CJValueP
CJRandFunction::
exec(CJavaScript *js, const Values &)
{
  double res = (1.0*rand())/RAND_MAX;

  return js->createNumberValue(res);
}

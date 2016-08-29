#include <CJRandFunction.h>
#include <CJavaScript.h>

CJValueP
CJRandFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 0) {
    js->errorMsg("Wrong number of arguments values");
    return CJValueP();
  }

  double res = (1.0*rand())/RAND_MAX;

  return js->createNumberValue(res);
}

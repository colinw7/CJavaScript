#include <CJIntervalFunction.h>
#include <CJavaScript.h>

CJSetInterval::
CJSetInterval(CJavaScript *js) :
 CJFunctionBase(js, "setInterval")
{
}

CJValueP
CJSetInterval::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 2)
    return CJValueP();

  // create timer to run proc every n miliseconds
  std::string proc     = values[0]->toString();
  double      interval = values[1]->toReal().getValue(0.0);

  long timer = js->setInterval(proc, interval);

  return js->createNumberValue(long(timer));
}

//------

CJClearInterval::
CJClearInterval(CJavaScript *js) :
 CJFunctionBase(js, "clearInterval")
{
}

CJValueP
CJClearInterval::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 1)
    return CJValueP();

  long timer = values[0]->toInteger().getValue(0);

  js->clearInterval(timer);

  return CJValueP();
}

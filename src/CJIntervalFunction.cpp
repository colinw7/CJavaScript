#include <CJIntervalFunction.h>
#include <CJavaScript.h>

CJSetInterval::
CJSetInterval(CJavaScript *js) :
 CJFunction(js, "setInterval")
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
  double      interval = values[1]->toReal();

  long timer = js->setInterval(proc, interval);

  return js->createNumberValue(long(timer));
}

//------

CJClearInterval::
CJClearInterval(CJavaScript *js) :
 CJFunction(js, "clearInterval")
{
}

CJValueP
CJClearInterval::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 1)
    return CJValueP();

  long timer = values[0]->toInteger();

  js->clearInterval(timer);

  return CJValueP();
}

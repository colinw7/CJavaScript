#include <CJIntervalFunction.h>
#include <CJavaScript.h>

CJSetInterval::
CJSetInterval(CJavaScript *js) :
 CJFunctionBase(js, "setInterval")
{
}

CJSetInterval::
CJSetInterval(const CJSetInterval &i) :
 CJFunctionBase(i)
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
  double      interval = values[1]->toReal().value_or(0.0);

  long timer = js->setInterval(proc, interval);

  return js->createNumberValue(long(timer));
}

//------

CJClearInterval::
CJClearInterval(CJavaScript *js) :
 CJFunctionBase(js, "clearInterval")
{
}

CJClearInterval::
CJClearInterval(const CJClearInterval &i) :
 CJFunctionBase(i)
{
}

CJValueP
CJClearInterval::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 1)
    return CJValueP();

  long timer = values[0]->toInteger().value_or(0);

  js->clearInterval(timer);

  return CJValueP();
}

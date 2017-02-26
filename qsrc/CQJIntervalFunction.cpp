#include <CQJIntervalFunction.h>
#include <CQJWindow.h>
#include <CJavaScript.h>

CQJSetInterval::
CQJSetInterval(CJavaScript *js, CQJWindowP window) :
 CJFunctionBase(js, "setInterval"), window_(window)
{
}

CJValueP
CQJSetInterval::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 2)
    return CJValueP();

  long timer = 0;

  // create timer to run proc every n miliseconds
  CJValueP fnValue = values[0];
  double   t       = values[1]->toReal().getValue(0);

  if (fnValue->type() == CJToken::Type::Function) {
    CJFunctionBaseP timerFn = CJValue::cast<CJFunctionBase>(fnValue);

    timer = window_->addTimer(timerFn, t);
  }
  else if (fnValue->type() == CJToken::Type::String) {
    CJStringP str = CJValue::cast<CJString>(fnValue);

    timer = window_->addTimer(str, t);
  }
  else {
    std::cerr << "Illegal Interval Function value" << std::endl;
  }

#if 0
  std::string proc     = values[0]->toString();
  double      interval = values[1]->toReal().getValue(0);

  timer = js->setInterval(proc, interval);
#endif

  return js->createNumberValue(long(timer));
}

//------

CQJClearInterval::
CQJClearInterval(CJavaScript *js, CQJWindowP window) :
 CJFunctionBase(js, "clearInterval"), window_(window)
{
}

CJValueP
CQJClearInterval::
exec(CJavaScript *, const Values &values)
{
  if (values.size() != 1)
    return CJValueP();

  long timer = values[0]->toInteger().getValue(0);

  window_->removeTimer(timer);

  //js->clearInterval(timer);

  return CJValueP();
}

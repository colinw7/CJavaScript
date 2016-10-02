#include <CQJIntervalFunction.h>
#include <CQJWindow.h>
#include <CQJavaScript.h>

CQJSetInterval::
CQJSetInterval(CQJavaScript *qjs) :
 CJFunctionBase(qjs->js(), "setInterval"), qjs_(qjs)
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
  double   t       = values[1]->toReal();

  if (fnValue->type() == CJToken::Type::Function) {
    CJFunctionBaseP timerFn = CJValue::cast<CJFunctionBase>(fnValue);

    CQJWindowP window = CJValue::cast<CQJWindow>(qjs_->jsWindow());

    timer = window->addTimer(timerFn, t);
  }

#if 0
  std::string proc     = values[0]->toString();
  double      interval = values[1]->toReal();

  timer = js->setInterval(proc, interval);
#endif

  return js->createNumberValue(long(timer));
}

//------

CQJClearInterval::
CQJClearInterval(CQJavaScript *qjs) :
 CJFunctionBase(qjs->js(), "clearInterval"), qjs_(qjs)
{
}

CJValueP
CQJClearInterval::
exec(CJavaScript *, const Values &values)
{
  if (values.size() != 1)
    return CJValueP();

  long timer = values[0]->toInteger();

  CQJWindowP window = CJValue::cast<CQJWindow>(qjs_->jsWindow());

  window->removeTimer(timer);

  //js->clearInterval(timer);

  return CJValueP();
}

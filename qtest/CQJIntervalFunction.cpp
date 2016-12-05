#include <CQJIntervalFunction.h>
#include <CQJWindow.h>
#include <CQJavaScript.h>

CQJSetInterval::
CQJSetInterval(CJavaScript *js) :
 CJFunctionBase(js, "setInterval")
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

    CQJavaScript *qjs = CQJavaScript::instance();

    CQJWindowP window = CJValue::cast<CQJWindow>(qjs->jsWindow());

    timer = window->addTimer(timerFn, t);
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
CQJClearInterval(CJavaScript *js) :
 CJFunctionBase(js, "clearInterval")
{
}

CJValueP
CQJClearInterval::
exec(CJavaScript *, const Values &values)
{
  if (values.size() != 1)
    return CJValueP();

  long timer = values[0]->toInteger().getValue(0);

  CQJavaScript *qjs = CQJavaScript::instance();

  CQJWindowP window = CJValue::cast<CQJWindow>(qjs->jsWindow());

  window->removeTimer(timer);

  //js->clearInterval(timer);

  return CJValueP();
}

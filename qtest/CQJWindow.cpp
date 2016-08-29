#include <CQJWindow.h>
#include <CQJWindowTimer.h>
#include <CQJavaScript.h>
#include <iostream>

CJObjTypeP CQJWindowType::type_;

CJObjTypeP
CQJWindowType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJWindowType(js));

    js->addObjectType("window", type_);
  }

  return type_;
}

CQJWindowType::
CQJWindowType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Window")
{
}

//------

CQJWindow::
CQJWindow(CQJavaScript *qjs) :
 CQJObject(qjs, CQJWindowType::instance(qjs->js()))
{
  CJavaScript *js = qjs->js();

  type_->addObjectFunction(js, "setTimeout");
}

long
CQJWindow::
addTimer(CJFunctionP timerFn, double t)
{
  CQJWindowP window = std::static_pointer_cast<CQJWindow>(shared_from_this());

  CQJWindowTimer *timer = new CQJWindowTimer(window, timerFn);

  timers_.push_back(timer);

  timer->start(t);

  return timer->id();
}

void
CQJWindow::
removeTimer(long id)
{
  for (uint i = 0; i < timers_.size(); ++i) {
    if (timers_[i]->id() != id)
      continue;

    delete timers_[i];

    timers_[i] = 0;
  }
}

void
CQJWindow::
addOneShotTimer(CJFunctionP timerFn, double t)
{
  CQJWindowP window = std::static_pointer_cast<CQJWindow>(shared_from_this());

  CQJWindowTimer *timer = new CQJWindowTimer(window, timerFn);

  timers_.push_back(timer);

  timer->setSingleShot(true);

  timer->start(t);
}

CJValueP
CQJWindow::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if (name == "setTimeout") {
    if (values.size() >= 2) {
      CJValueP fnValue = values[1];
      double   t       = values[2]->toReal();

      if (fnValue->type() == CJToken::Type::Function) {
        CJFunctionP timerFn = std::static_pointer_cast<CJFunction>(fnValue);

        if (! timer_) {
          CQJWindowP window = std::static_pointer_cast<CQJWindow>(shared_from_this());

          timer_ = new CQJWindowTimer(window, timerFn);
        }
        else
          timer_->setFunction(timerFn);

        timer_->start(t);
      }
    }

    return CJValueP();
  }
  else
    return CQJObject::execNameFn(js, name, values);
}

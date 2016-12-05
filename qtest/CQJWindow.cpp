#include <CQJWindow.h>
#include <CQJWindowTimer.h>
#include <CQJCanvasWidget.h>
#include <CQJRequestAnimationFrame.h>
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
CQJWindow(CJavaScript *js) :
 CQJObject(js, CQJWindowType::instance(js))
{
  objType_->addObjFunction(js, "setTimeout", objType_);

  addPseudoProperty("innerWidth");
  addPseudoProperty("innerHeight");

  setProperty(js, "requestAnimationFrame", CJValueP(new CQJRequestAnimationFrame(js)));
}

long
CQJWindow::
addTimer(CJFunctionBaseP timerFn, double t)
{
  CQJWindowP window = CJValue::cast<CQJWindow>(shared_from_this());

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
addOneShotTimer(CJFunctionBaseP timerFn, double t)
{
  CQJWindowP window = CJValue::cast<CQJWindow>(shared_from_this());

  CQJWindowTimer *timer = new CQJWindowTimer(window, timerFn);

  timers_.push_back(timer);

  timer->setSingleShot(true);

  timer->start(t);
}

CJValueP
CQJWindow::
getProperty(CJavaScript *js, const std::string &name) const
{
  if       (name == "innerWidth") {
    CQJavaScript *qjs = CQJavaScript::instance();

    return js->createNumberValue(long(qjs->canvas()->width()));
  }
  else if  (name == "innerHeight") {
    CQJavaScript *qjs = CQJavaScript::instance();

    return js->createNumberValue(long(qjs->canvas()->height()));
  }
  else
    return CJObj::getProperty(js, name);
}

void
CQJWindow::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJObj::setProperty(js, name, value);
}

CJValueP
CQJWindow::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if (name == "setTimeout") {
    if (values.size() >= 2) {
      CJValueP fnValue = values[1];
      double   t       = values[2]->toReal().getValue(0);

      if (fnValue->type() == CJToken::Type::Function) {
        CJFunctionBaseP timerFn = CJValue::cast<CJFunctionBase>(fnValue);

        if (! timer_) {
          CQJWindowP window = CJValue::cast<CQJWindow>(shared_from_this());

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

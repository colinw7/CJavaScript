#include <CQJWindow.h>
#include <CQJavaScript.h>
#include <QTimer>
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

  timer_ = new QTimer(this);

  connect(timer_, SIGNAL(timeout()), this, SLOT(timerSlot()));
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
        timerFn_ = std::static_pointer_cast<CJFunction>(fnValue);

        timer_->start(t);
      }
    }

    return CJValueP();
  }
  else
    return CQJObject::execNameFn(js, name, values);
}

void
CQJWindow::
timerSlot()
{
  CJavaScript *js = js_->js();

  CJObjType::Values fnValues;

  fnValues.push_back(shared_from_this());

  timerFn_->exec(js, fnValues);
}

#include <CQJWindow.h>
#include <CQJavaScript.h>
#include <QTimer>
#include <iostream>

CQJWindowType::
CQJWindowType() :
 CJObjectType(CJToken::Type::Object, "Window")
{
}

//------

CQJWindow::
CQJWindow(CQJavaScript *qjs) :
 CQJObject(qjs, qjs->jsWindowType())
{
  CJavaScript *js = qjs->js();

  type_->addFunction(js, "setTimeout");

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

  CJObjectType::Values fnValues;

  fnValues.push_back(shared_from_this());

  timerFn_->exec(js, fnValues);
}

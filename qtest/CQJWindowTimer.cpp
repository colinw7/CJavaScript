#include <CQJWindowTimer.h>
#include <CQJavaScript.h>

int CQJWindowTimer::lastId_ = 0;

CQJWindowTimer::
CQJWindowTimer(CQJWindowP window, CJFunctionP timerFn) :
 window_(window), timerFn_(timerFn), id_(++lastId_)
{
  connect(this, SIGNAL(timeout()), this, SLOT(timerSlot()));
}

void
CQJWindowTimer::
timerSlot()
{
  CJavaScript *js = window_->qjs()->js();

  CJObjType::Values fnValues;

  fnValues.push_back(window_);

  timerFn_->exec(js, fnValues);
}

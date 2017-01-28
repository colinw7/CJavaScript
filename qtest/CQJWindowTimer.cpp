#include <CQJWindowTimer.h>
#include <CJavaScript.h>

int CQJWindowTimer::lastId_ = 0;

CQJWindowTimer::
CQJWindowTimer(CQJWindowP window, CJFunctionBaseP timerFn) :
 window_(window), timerFn_(timerFn), id_(++lastId_)
{
  connect(this, SIGNAL(timeout()), this, SLOT(timerSlot()));
}

void
CQJWindowTimer::
timerSlot()
{
  CJavaScript *js = window_->js();

  CJObjType::Values fnValues;

  fnValues.push_back(window_);

  timerFn_->exec(js, fnValues);
}

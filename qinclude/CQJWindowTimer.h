#ifndef CQJWindowTimer_H
#define CQJWindowTimer_H

#include <CQJWindow.h>

#include <QTimer>

class CQJWindowTimer : public QTimer {
  Q_OBJECT

 public:
  CQJWindowTimer(CQJWindowP window, CJFunctionBaseP timerFn);
  CQJWindowTimer(CQJWindowP window, CJStringP timerStr);

  int id() const { return id_; }
  void setId(int id) { id_ = id; }

  void setFunction(CJFunctionBaseP fn) { timerFn_ = fn; }
  void setFunction(CJStringP str) { timerStr_ = str; }

 private slots:
  void timerSlot();

 private:
  static int lastId_;

  CQJWindowP      window_;
  CJFunctionBaseP timerFn_;
  CJStringP       timerStr_;
  int             id_ { 0 };
};

#endif

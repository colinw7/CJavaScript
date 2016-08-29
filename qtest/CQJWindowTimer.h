#ifndef CQJWindowTimer_H
#define CQJWindowTimer_H

#include <CQJWindow.h>

#include <QTimer>

class CQJWindowTimer : public QTimer {
  Q_OBJECT

 public:
  CQJWindowTimer(CQJWindowP window, CJFunctionP timerFn);

  int id() const { return id_; }
  void setId(int id) { id_ = id; }

  void setFunction(CJFunctionP fn) { timerFn_ = fn; }

 private slots:
  void timerSlot();

 private:
  static int lastId_;

  CQJWindowP  window_;
  CJFunctionP timerFn_;
  int         id_ { 0 };
};

#endif

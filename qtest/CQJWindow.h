#ifndef CQJWindow_H
#define CQJWindow_H

#include <CQJObject.h>

class CQJWindowType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CQJWindowType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJWindow;
class CQJWindowTimer;

typedef std::shared_ptr<CQJWindow> CQJWindowP;

//------

class CQJWindow : public CQJObject {
  Q_OBJECT

 public:
  CQJWindow(CQJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CQJWindow(qjs_); }

  long addTimer(CJFunctionP timerFn, double t);
  void removeTimer(long id);

  void addOneShotTimer(CJFunctionP timerFn, double t);

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "window"; }

 private:
  typedef std::vector<CQJWindowTimer *> Timers;

  CQJWindowTimer *timer_ { 0 };
  Timers          timers_;
};

#endif

#ifndef CQJWindow_H
#define CQJWindow_H

#include <CQJObject.h>

class QTimer;

class CQJWindowType : public CJObjectType {
 public:
  CQJWindowType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CQJWindow : public CQJObject {
  Q_OBJECT

 public:
  CQJWindow(CQJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CQJWindow(js_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "window"; }

 private slots:
  void timerSlot();

 private:
  QTimer*     timer_ { 0 };
  CJFunctionP timerFn_;
};

#endif

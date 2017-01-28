#ifndef CQJEvent_H
#define CQJEvent_H

#include <CQJObject.h>
#include <QKeyEvent>

class CQJEventType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJEventType(CJavaScript *js);

  bool hasConstructor() const { return true; }

  CJValueP construct(CJavaScript *js, const Values &values) override;

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJEvent : public CQJObject {
 public:
  CQJEvent(CJavaScript *js, CJValueP value, QEvent *e);

  CJValue *dup(CJavaScript *js) const override { return new CQJEvent(js, value_, e_); }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;
  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "event"; }

 private:
  CJValueP value_;
  QEvent*  e_ { 0 };
};

#endif

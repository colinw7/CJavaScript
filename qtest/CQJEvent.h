#ifndef CQJEvent_H
#define CQJEvent_H

#include <CQJObject.h>
#include <QKeyEvent>

class CQJavaScript;

class CQJEventType : public CJObjType {
 public:
  static CJObjTypeP instance(CQJavaScript *js);

  CQJEventType(CQJavaScript *js);

  bool hasConstructor() const { return true; }

  CJValueP construct(CJavaScript *js, const Values &values) override;

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;

  CQJavaScript *qjs_ { 0 };
};

//------

class CQJEvent : public CQJObject {
 public:
  CQJEvent(CQJavaScript *js, QEvent *e);

  CJValue *dup(CJavaScript *) const override { return new CQJEvent(qjs_, e_); }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;
  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "event"; }

 private:
  QEvent *e_ { 0 };
};

#endif

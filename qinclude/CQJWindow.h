#ifndef CQJWindow_H
#define CQJWindow_H

#include <CQJObject.h>

class CQJWindowType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJWindowType(CJavaScript *js);

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

#include <CQJDocument.h>
#include <CQJLocation.h>

class CQJWindowIFace;

//------

class CQJWindow : public CQJObject {
  Q_OBJECT

 public:
  CQJWindow(CJavaScript *js, CQJWindowIFace *iface);

  void init() override;

  CJValue *dup(CJavaScript *js) const override { return new CQJWindow(js, iface_); }

  CQJDocumentP document() const;

  long addTimer(CJFunctionBaseP timerFn, double t);
  long addTimer(CJStringP timerStr, double t);

  void removeTimer(long id);

  void addOneShotTimer(CJFunctionBaseP timerFn, double t);

  CQJLocationP location() const { return location_; }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const;

  void setProperty(CJavaScript *js, const std::string &name, CJValueP value);

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "window"; }

 private:
  typedef std::vector<CQJWindowTimer *> Timers;

  CQJWindowIFace *iface_ { nullptr };
  CQJWindowTimer *timer_ { nullptr };
  Timers          timers_;
  CQJLocationP    location_;
};

#endif

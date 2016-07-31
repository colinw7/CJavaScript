#ifndef CQJObject_H
#define CQJObject_H

#include <QObject>
#include <CJavaScript.h>

class CQJavaScript;

class CQJObject : public QObject, public CJObject, public std::enable_shared_from_this<CQJObject> {
  Q_OBJECT

 public:
  CQJObject(CQJavaScript *js, const CJObjectTypeP &type);
 ~CQJObject();

  CJValue *dup(CJavaScript *) const override { return new CQJObject(js_, type()); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void callEventListener(const std::string &name);

  void print(std::ostream &os) const override { os << "object"; }

 protected:
  typedef std::map<std::string,CJValueP> EventListeners;

  CQJavaScript*  js_ { 0 };
  EventListeners eventListeners_;
};

#endif

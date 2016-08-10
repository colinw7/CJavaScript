#ifndef CQJObject_H
#define CQJObject_H

#include <QObject>
#include <CJavaScript.h>

class CQJavaScript;

class CQJObject : public QObject, public CJObject {
  Q_OBJECT

 public:
  typedef std::vector<CJValueP>           EventArgs;
  typedef std::pair<std::string,CJValueP> NameValue;
  typedef std::vector<NameValue>          NameValues;

 public:
  CQJObject(CQJavaScript *js, const CJObjectTypeP &type);
 ~CQJObject();

  CQJavaScript *js() const { return js_; }

  CJValue *dup(CJavaScript *) const override { return new CQJObject(js_, type()); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void callEventListener(const std::string &name, const EventArgs &args=EventArgs(),
                         const NameValues &nameValues=NameValues());

  void print(std::ostream &os) const override { os << "object"; }

 protected:
  typedef std::map<std::string,CJValueP> EventListeners;

  CQJavaScript*  js_ { 0 };
  EventListeners eventListeners_;
};

#endif

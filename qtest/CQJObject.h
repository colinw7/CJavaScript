#ifndef CQJObject_H
#define CQJObject_H

#include <QObject>
#include <CJavaScript.h>

class CQJavaScript;

class CQJObject : public QObject, public CJObj {
  Q_OBJECT

 public:
  typedef std::vector<CJValueP>           EventArgs;
  typedef std::pair<std::string,CJValueP> NameValue;
  typedef std::vector<NameValue>          NameValues;

 public:
  CQJObject(CQJavaScript *js, const CJObjTypeP &type);
 ~CQJObject();

  CQJavaScript *qjs() const { return qjs_; }

  CJValue *dup(CJavaScript *) const override { return new CQJObject(qjs_, objType()); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(0); }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  bool callEventListener(const std::string &name, const std::string &prop,
                         const EventArgs &args=EventArgs(),
                         const NameValues &nameValues=NameValues());

  void print(std::ostream &os) const override { os << "object"; }

 private:
  bool callEventListener(CJValueP value, const EventArgs &args, const NameValues &nameValues);

 protected:
  typedef std::map<std::string,CJValueP> EventListeners;

  CQJavaScript*  qjs_ { 0 };
  EventListeners eventListeners_;
};

#endif

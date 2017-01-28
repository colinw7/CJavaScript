#ifndef CQJObject_H
#define CQJObject_H

#include <CJObj.h>
#include <QObject>

class CQJObject : public QObject, public CJObj {
  Q_OBJECT

 public:
  typedef std::vector<CJValueP>           EventArgs;
  typedef std::pair<std::string,CJValueP> NameValue;
  typedef std::vector<NameValue>          NameValues;

 public:
  CQJObject(CJavaScript *js, const CJObjTypeP &type);

  virtual ~CQJObject();

  virtual void init() { }

  CJValue *dup(CJavaScript *js) const override { return new CQJObject(js, objType()); }

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

  EventListeners eventListeners_;
};

#endif

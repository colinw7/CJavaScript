#ifndef CJObject_H
#define CJObject_H

#include <CJValue.h>
#include <CJNameSpace.h>
#include <set>
#include <algorithm>

class CJObject;

typedef std::shared_ptr<CJObject> CJObjectP;

class CJObject : public CJValue, public CJNameSpace {
 public:
  typedef std::set<std::string> Names;
  typedef std::vector<CJValueP> Values;

 public:
  CJObject(CJObjectTypeP type) :
   CJValue(type), type_(type) {
  }

  CJObjectTypeP type() const { return type_; }

  const std::string &name() const;

  std::string toString() const override {
    return "[object " + name() + "]";
  }

  bool toBoolean() const override { return true; }

  double toReal() const override { return toBoolean(); }

  virtual int cmp(CJObjectP obj) const {
    if (this < obj.get()) return -1;
    if (this > obj.get()) return  1;
    return 0;
  }

  CJValueP indexValue(const std::string &key) const { return CJNameSpace::getProperty(key); }

  void setIndexValue(CJavaScript *, const std::string &key, CJValueP value) {
    setProperty(key, value);
  }

  void setIndexValue(CJValueP ivalue, CJValueP value) {
    std::string key = ivalue->toString();

    for (auto &kv : keyValues_) {
      if (kv.first == key) {
        kv.second = value;
        return;
      }
    }
  }

  void addVariable(CJavaScript *js, const std::string &name);

  virtual CJValueP execNameFn(CJavaScript *, const std::string &, const Values &);

  void print(std::ostream &os) const override {
    os << toString();
  }

 protected:
  CJObjectTypeP type_;
};

#endif

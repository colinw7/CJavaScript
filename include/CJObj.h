#ifndef CJObj_H
#define CJObj_H

#include <CJObjType.h>
#include <CJToken.h>
#include <set>
#include <algorithm>
#include <CJDictionary.h>

class CJObj : public CJDictionary {
 public:
  typedef std::set<std::string> Names;
  typedef std::vector<CJValueP> Values;

 public:
  CJObj(CJavaScript *js, CJObjTypeP type);

  CJValue *dup(CJavaScript *js) const override {
    return new CJObj(js, type_);
  }

  bool isObject() const override { return true; }

  CJObjTypeP type() const { return type_; }

  const std::string &name() const;

  std::string toString() const override {
    return "[object " + name() + "]";
  }

  bool toBoolean() const override { return true; }

  virtual int cmp(CJObjP obj) const {
    if (this < obj.get()) return -1;
    if (this > obj.get()) return  1;

    return 0;
  }

  void addVariable(CJavaScript *js, const std::string &name);

  CJValueP propertyValue(const std::string &key) const override;
  void setPropertyValue(const std::string &key, CJValueP value) override;

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;
  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  virtual CJValueP execNameFn(CJavaScript *, const std::string &, const Values &);

  void print(std::ostream &os) const override {
    os << toString();
  }

  friend std::ostream &operator<<(std::ostream &os, const CJObj &rhs) {
    rhs.print(os);

    return os;
  }

 protected:
  CJObjTypeP type_;
};

#endif

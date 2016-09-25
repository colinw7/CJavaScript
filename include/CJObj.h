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
  CJObj(CJavaScript *js, CJObjTypeP objType);

  CJValue *dup(CJavaScript *js) const override {
    return new CJObj(js, objType_);
  }

  bool isObject() const override { return true; }

  CJObjTypeP objType() const { return objType_; }

  const std::string &name() const;

  std::string toString() const override;

  bool toBoolean() const override { return true; }

  virtual int cmp(CJObjP obj) const {
    if (this < obj.get()) return -1;
    if (this > obj.get()) return  1;

    return 0;
  }

  void addVariable(CJavaScript *js, const std::string &name);

  bool hasPropertyValue(const std::string &key) const override;
  CJValueP propertyValue(const std::string &key) const override;
  void setPropertyValue(const std::string &key, CJValueP value) override;

  bool hasProperty(CJavaScript *js, const std::string &key) const override;
  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  bool isEnumerableProperty(const std::string &key) const override;

  CJValue::KeyNames propertyNames() const override;

  virtual CJValueP execNameFn(CJavaScript *, const std::string &, const Values &);

  void print(std::ostream &os) const override;

  friend std::ostream &operator<<(std::ostream &os, const CJObj &rhs) {
    rhs.print(os);

    return os;
  }

 private:
  CJValueP protoValue() const;

 protected:
  CJObjTypeP objType_;
};

#endif

#ifndef CJObj_H
#define CJObj_H

#include <CJObjType.h>
#include <CJDictionary.h>
#include <CJToken.h>
#include <set>
#include <algorithm>

class CJObj : public CJDictionary {
 public:
  typedef std::set<std::string> Names;
  typedef std::vector<CJValueP> Values;

 public:
  CJObj(CJavaScript *js, CJObjTypeP objType);

  CJObj(const CJObj &obj);

  CJValue *dup(CJavaScript *js) const override;

  bool isObject() const override { return true; }

  CJObjTypeP objType() const { return objType_; }

  const std::string &name() const;

  bool toBoolean() const override { return true; }

  virtual bool isNative() const { return true; }

  virtual bool isHost() const { return false; }

  virtual bool isUserDefined() const { return false; }

  virtual int cmp(CJObjP obj) const {
    if (this < obj.get()) return -1;
    if (this > obj.get()) return  1;

    return 0;
  }

  void addVariable(CJavaScript *js, const std::string &name);

  bool hasPropertyValue(const std::string &key, bool inherit=true) const override;
  CJValueP propertyValue(const std::string &key) const override;
  void setPropertyValue(const std::string &key, CJValueP value) override;
  void configPropertyValue(const std::string &key, CJValueP value) override;

  bool hasProperty(CJavaScript *js, const std::string &key, bool inherit=true) const override;
  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  bool isEnumerableProperty(const std::string &key) const override;

  CJValue::KeyNames propertyNames() const override;

  bool isProtoValue(CJValueP value) const;

  CJValueP protoValue() const;
  void setProtoValue(CJValueP value);

  bool isExtensible() const { return extensible_; }
  void setExtensible(bool b) { extensible_ = b; }

  bool isFrozen() const { return frozen_; }
  void setFrozen(bool b) { frozen_ = b; }

  bool isSealed() const { return sealed_; }
  void setSealed(bool b) { sealed_ = b; }

  virtual CJValueP execNameFn(CJavaScript *, const std::string &, const Values &);

  std::string toString() const override;

  void print(std::ostream &os) const override;

  friend std::ostream &operator<<(std::ostream &os, const CJObj &rhs) {
    rhs.print(os);

    return os;
  }

 protected:
  CJObjTypeP objType_;
  CJValueP   protoValue_;
  bool       extensible_ { true };
  bool       frozen_ { false };
  bool       sealed_ { false };
};

#endif

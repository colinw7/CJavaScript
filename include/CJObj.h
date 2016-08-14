#ifndef CJObj_H
#define CJObj_H

#include <CJValue.h>
#include <CJNameSpace.h>
#include <CJToken.h>
#include <set>
#include <algorithm>

class CJObj;

typedef std::shared_ptr<CJObj> CJObjP;

//------

class CJObjType : public CJNameSpace, public std::enable_shared_from_this<CJObjType> {
 public:
  typedef std::set<std::string> Functions;
  typedef std::vector<CJValueP> Values;

 public:
  CJObjType(CJavaScript *js, const CJToken::Type type, const std::string &name);

  const CJToken::Type &type() const { return type_; }

  const std::string &name() const { return name_; }

  void addTypeFunction(CJavaScript *js, const std::string &name);
  void addObjectFunction(CJavaScript *js, const std::string &name);

  virtual bool hasConstructor() const { return false; }

  virtual CJValueP construct(CJavaScript *, const Values &) { return CJValueP(); }

  CJValueP getProperty(const std::string &key) const override;

  KeyNames getTypePropertyNames() const;

  virtual CJValueP exec(CJavaScript *, const std::string &, const Values &) {
    return CJValueP();
  }

  virtual void print(std::ostream &os) const {
    os << name_;
  }

  friend std::ostream &operator<<(std::ostream &os, const CJObjType &rhs) {
    rhs.print(os);

    return os;
  }

 protected:
  CJavaScript*  js_ { 0 };
  CJToken::Type type_;
  std::string   name_;
};

typedef std::shared_ptr<CJObjType> CJObjTypeP;

//------

class CJObj : public CJValue, public CJNameSpace {
 public:
  typedef std::set<std::string> Names;
  typedef std::vector<CJValueP> Values;

 public:
  CJObj(CJObjTypeP type);

  bool isObject() const override { return true; }

  CJObjTypeP type() const { return type_; }

  const std::string &name() const;

  std::string toString() const override {
    return "[object " + name() + "]";
  }

  bool toBoolean() const override { return true; }

  double toReal() const override { return toBoolean(); }

  virtual int cmp(CJObjP obj) const {
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
  CJObjTypeP type_;
};

#endif

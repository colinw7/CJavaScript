#ifndef CJGetterSetter_H
#define CJGetterSetter_H

#include <CJObjType.h>
#include <CJValue.h>
#include <CJNameSpace.h>
#include <sstream>

// GetterSetter Type
class CJGetterSetterType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CJGetterSetterType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//------

// GetterSetter Value
class CJGetterSetter : public CJValue, public CJNameSpace {
 public:
  CJGetterSetter(CJavaScript *js, CJFunctionP getter=CJFunctionP(),
                 CJFunctionP setter=CJFunctionP());

  CJGetterSetter(const CJGetterSetter &gs);

  CJValue *dup(CJavaScript *) const override { return new CJGetterSetter(*this); }

  CJavaScript *js() const { return js_; }

  CJFunctionP getter() const { return getter_; }
  void setGetter(CJFunctionP f) { getter_ = f; }

  CJFunctionP setter() const { return setter_; }
  void setSetter(CJFunctionP f) { setter_ = f; }

  const CJValueP &parent() const { return value_; }
  void setParent(const CJValueP &v) { value_ = v; }

  CJValueP getValue() const;
  void setValue(CJValueP value);

  //---

  bool toBoolean() const override { return false; }

  OptReal toReal() const override { return OptReal(); }

  OptLong toInteger() const override { return OptLong(); }

  //---

  std::string toString() const override { return ""; }

  void print(std::ostream &os) const override;

  friend std::ostream &operator<<(std::ostream &os, const CJGetterSetter &gs) {
    gs.print(os);

    return os;
  }

 protected:
  CJavaScript* js_ { 0 };
  CJFunctionP  getter_;
  CJFunctionP  setter_;
  CJValueP     value_;
};

#endif

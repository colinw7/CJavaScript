#ifndef CJFalse_H
#define CJFalse_H

#include <CJObj.h>
#include <CJValue.h>

// False Type
class CJFalseType : public CJObjType {
 public:
  static CJFalseTypeP instance(CJavaScript *js);

  explicit CJFalseType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJFalseTypeP type_;
};

//------

// False Value
class CJFalse : public CJValue {
 public:
  static CJFalseP value(CJavaScript *js);

 public:
  explicit CJFalse(CJavaScript *js);

  CJFalse(const CJFalse &f);

  bool isPrimitive() const override { return true; }

  CJValue *dup(CJavaScript *) const override { return new CJFalse(*this); }

  //---

  COptReal toReal() const override { return COptReal(0); }

  COptLong toInteger() const override { return COptLong(0); }

  bool toBoolean() const override { return false; }

  //---

  std::string toString() const override { return "false"; }

  void print(std::ostream &os) const override;
};

#endif

#ifndef CJTrue_H
#define CJTrue_H

#include <CJObj.h>
#include <CJValue.h>

// True Type
class CJTrueType : public CJObjType {
 public:
  static CJTrueTypeP instance(CJavaScript *js);

  explicit CJTrueType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJTrueTypeP type_;
};

// True Value
class CJTrue : public CJValue {
 public:
  static CJTrueP value(CJavaScript *js);

 public:
  explicit CJTrue(CJavaScript *js);

  CJTrue(const CJTrue &t);

  bool isPrimitive() const override { return true; }

  CJValue *dup(CJavaScript *) const override { return new CJTrue(*this); }

  //---

  OptReal toReal() const override { return OptReal(1); }

  OptLong toInteger() const override { return OptLong(1); }

  bool toBoolean() const override { return true; }

  //---

  std::string toString() const override { return "true"; }

  void print(std::ostream &os) const override;
};

#endif

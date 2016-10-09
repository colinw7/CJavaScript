#ifndef CJTrue_H
#define CJTrue_H

#include <CJObj.h>
#include <CJValue.h>

// True Type
class CJTrueType : public CJObjType {
 public:
  static CJTrueTypeP instance(CJavaScript *js);

  CJTrueType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJTrueTypeP type_;
};

// True Value
class CJTrue : public CJValue {
 public:
  static CJTrueP value(CJavaScript *js);

 public:
  CJTrue(CJavaScript *js);

  bool isPrimitive() const override { return true; }

  CJValue *dup(CJavaScript *js) const override { return new CJTrue(js); }

  //---

  COptReal toReal() const override { return COptReal(1); }

  COptLong toInteger() const override { return COptLong(1); }

  bool toBoolean() const override { return true; }

  //---

  std::string toString() const override { return "true"; }

  void print(std::ostream &os) const override;
};

#endif

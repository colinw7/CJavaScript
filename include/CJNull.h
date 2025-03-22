#ifndef CJNull_H
#define CJNull_H

#include <CJObj.h>
#include <CJValue.h>

// Null Type
class CJNullType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CJNullType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//------

// Null Value
class CJNull : public CJValue {
 public:
  static CJValueP value(CJavaScript *js);

 private:
  explicit CJNull(CJavaScript *js);

  CJNull(const CJNull &n);

 public:
  CJValue *dup(CJavaScript *) const override { return new CJNull(*this); }

  //---

  OptReal toReal() const override { return OptReal(0); }

  OptLong toInteger() const override { return OptLong(0); }

  bool toBoolean() const override { return false; }

  //---

  std::string toString() const override { return "null"; }

  void print(std::ostream &os) const override;
};

#endif

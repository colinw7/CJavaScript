#ifndef CJUndefined_H
#define CJUndefined_H

#include <CJObj.h>
#include <CJValue.h>

// Undefined Type
class CJUndefinedType : public CJObjType {
 public:
  static CJUndefinedTypeP instance(CJavaScript *js);

  CJUndefinedType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJUndefinedTypeP type_;
};

// Undefined Value
class CJUndefined : public CJValue {
 public:
  static CJUndefinedP value(CJavaScript *js);

 public:
  CJUndefined(CJavaScript *js);

  CJUndefined(const CJUndefined &undef);

  CJValue *dup(CJavaScript *) const override { return new CJUndefined(*this); }

  //---

  COptLong toInteger() const override { return COptLong(); }

  COptReal toReal() const override { return COptReal(); }

  bool toBoolean() const override { return false; }

  //---

  std::string toString() const override { return "undefined"; }

  void print(std::ostream &os) const override;
};

#endif

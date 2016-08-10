#ifndef CJUndefined_H
#define CJUndefined_H

#include <CJObjectType.h>
#include <CJValue.h>

// Undefined Type
class CJUndefinedType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CJUndefinedType();

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjectTypeP type_;
};

// Undefined Value
class CJUndefined : public CJValue {
 public:
  CJUndefined(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJUndefined(js); }

  std::string toString() const override { return "undefined"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const override { os << "undefined"; }
};

#endif

#ifndef CJFalse_H
#define CJFalse_H

#include <CJObjectType.h>
#include <CJValue.h>

// False Type
class CJFalseType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CJFalseType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjectTypeP type_;
};

//------

// False Value
class CJFalse : public CJValue {
 public:
  CJFalse(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJFalse(js); }

  bool isProtoValue() const override { return true; }

  std::string toString() const override { return "false"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const override { os << "false"; }
};

#endif

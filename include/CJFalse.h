#ifndef CJFalse_H
#define CJFalse_H

#include <CJObj.h>
#include <CJValue.h>

// False Type
class CJFalseType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJFalseType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//------

// False Value
class CJFalse : public CJValue {
 public:
  static CJValueP value(CJavaScript *js);

 private:
  CJFalse(CJavaScript *js);

 public:
  CJValue *dup(CJavaScript *js) const override { return new CJFalse(js); }

  bool isProtoValue() const override { return true; }

  std::string toString() const override { return "false"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const override { os << "false"; }
};

#endif

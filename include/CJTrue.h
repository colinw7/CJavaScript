#ifndef CJTrue_H
#define CJTrue_H

#include <CJObjectType.h>
#include <CJValue.h>

// True Type
class CJTrueType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CJTrueType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjectTypeP type_;
};

// True Value
class CJTrue : public CJValue {
 public:
  CJTrue(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJTrue(js); }

  bool isProtoValue() const override { return true; }

  std::string toString() const override { return "true"; }

  double toReal() const override { return 1; }

  bool toBoolean() const override { return true; }

  void print(std::ostream &os) const override { os << "true"; }
};

#endif

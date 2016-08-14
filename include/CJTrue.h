#ifndef CJTrue_H
#define CJTrue_H

#include <CJObj.h>
#include <CJValue.h>

// True Type
class CJTrueType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJTrueType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

// True Value
class CJTrue : public CJValue {
 public:
  static CJValueP value(CJavaScript *js);

 private:
  CJTrue(CJavaScript *js);

 public:
  CJValue *dup(CJavaScript *js) const override { return new CJTrue(js); }

  bool isProtoValue() const override { return true; }

  std::string toString() const override { return "true"; }

  double toReal() const override { return 1; }

  bool toBoolean() const override { return true; }

  void print(std::ostream &os) const override { os << "true"; }
};

#endif

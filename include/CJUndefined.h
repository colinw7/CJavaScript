#ifndef CJUndefined_H
#define CJUndefined_H

#include <CJObj.h>
#include <CJValue.h>

// Undefined Type
class CJUndefinedType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJUndefinedType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

// Undefined Value
class CJUndefined : public CJValue {
 public:
  static CJValueP value(CJavaScript *js);

 private:
  CJUndefined(CJavaScript *js);

 public:
  CJValue *dup(CJavaScript *js) const override { return new CJUndefined(js); }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  std::string toString() const override { return "undefined"; }

  void print(std::ostream &os) const override;
};

#endif

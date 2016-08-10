#ifndef CJNull_H
#define CJNull_H

#include <CJObjectType.h>
#include <CJValue.h>

// Null Type
class CJNullType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CJNullType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjectTypeP type_;
};

//------

// Null Value
class CJNull : public CJValue {
 public:
  CJNull(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJNull(js); }

  std::string toString() const override { return "null"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const override { os << "null"; }
};

#endif

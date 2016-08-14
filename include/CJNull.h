#ifndef CJNull_H
#define CJNull_H

#include <CJObj.h>
#include <CJValue.h>

// Null Type
class CJNullType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJNullType(CJavaScript *js);

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
  CJNull(CJavaScript *js);

 public:
  CJValue *dup(CJavaScript *js) const override { return new CJNull(js); }

  std::string toString() const override { return "null"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const override { os << "null"; }
};

#endif

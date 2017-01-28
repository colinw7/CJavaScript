#ifndef CJNumber_H
#define CJNumber_H

#include <CJObj.h>
#include <CJUtil.h>
#include <limits>
#include <sstream>

// Number Type
class CJNumberType : public CJObjType {
 public:
  static CJNumberTypeP instance(CJavaScript *js);

  explicit CJNumberType(CJavaScript *js);

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;

  CJValueP execType(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  void init();

 private:
  static CJNumberTypeP type_;
};

//------

// Number Value
class CJNumber : public CJObj {
 public:
  enum class RealType {
    NaN,
    PosInf,
    NegInf
  };

 public:
  CJNumber(CJavaScript *js, double real=0.0);

  CJNumber(const CJNumber &n);

  CJNumber *dup(CJavaScript *) const override { return new CJNumber(*this); }

  bool isPrimitive() const override { return primitive_; }
  void setIsPrimitive(bool b) { primitive_ = b; }

  bool isNumber() const override { return true; }

  std::string toString() const override;

  double real() const { return real_; }
  void setReal(double r) { real_ = r; }

  bool isInteger() const { return CJUtil::isInteger(real_); }

  //---

  bool toBoolean() const override { return (real_ != 0.0); }

  COptReal toReal() const override { return COptReal(real_); }

  COptLong toInteger() const override;

  //---

  bool hasProperty() const override { return ! isPrimitive(); }

  std::string realString() const;

  int cmp(const CJValue *v) const override {
    double r1 =    toReal().getValue(0);
    double r2 = v->toReal().getValue(0);

    if (r1 < r2) return -1;
    if (r1 > r2) return  1;

    return 0;
  }

 private:
  double real_ { 0.0 };
  bool   primitive_ { true };
};

#endif

#ifndef CJNumber_H
#define CJNumber_H

#include <CJObj.h>
#include <limits>
#include <sstream>

// Number Type
class CJNumberType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJNumberType(CJavaScript *js);

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//------

// Number Value
class CJNumber : public CJObj {
 public:
  CJNumber(CJavaScript *js, double real=0.0);

  CJNumber *dup(CJavaScript *js) const override { return new CJNumber(js, real_); }

  std::string toString() const override;

  double real() const { return real_; }
  void setReal(double r) { real_ = r; }

  double toReal() const override { return real_; }

  bool toBoolean() const override { return (real_ != 0.0); }

  int cmp(const CJValue *v) const override {
    double r1 =    toReal();
    double r2 = v->toReal();

    if (r1 < r2) return -1;
    if (r1 > r2) return  1;

    return 0;
  }

 private:
  double real_;
};

#endif

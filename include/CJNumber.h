#ifndef CJNumber_H
#define CJNumber_H

#include <CJObjectType.h>
#include <CJValue.h>
#include <limits>
#include <sstream>

// Number Type
class CJNumberType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CJNumberType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjectTypeP type_;
};

//------

// Number Value
class CJNumber : public CJValue {
 public:
  CJNumber(CJavaScript *js, double real=0.0);

  CJNumber *dup(CJavaScript *js) const override { return new CJNumber(js, real_); }

  std::string toString() const override {
    std::ostringstream ss;

    ss.precision(std::numeric_limits<double>::max_digits10);

    ss << real_;

    return ss.str();
  }

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

  void print(std::ostream &os) const override {
    os << toString();
  }

 private:
  double real_;
};

typedef std::shared_ptr<CJNumber> CJNumberP;

#endif

#ifndef CQJCanvasRadialGradient_H
#define CQJCanvasRadialGradient_H

#include <CJObjectType.h>
#include <CJObject.h>

class CQJCanvasRadialGradientType : public CJObjectType {
 public:
  static CJObjectTypeP instance();

  CQJCanvasRadialGradientType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjectTypeP type_;
};

class CQJCanvasRadialGradient : public CJObject {
 public:
  CQJCanvasRadialGradient(CQJavaScript *js, double x1=0, double y1=0, double r1=1,
                          double x2=0, double y2=0, double r2=1);

  CQJavaScript *js() const { return js_; }

  CJValue *dup(CJavaScript *) const override { return new CQJCanvasRadialGradient(js_); }

  const QRadialGradient &rg() const { return rg_; }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "canvasRadialGradient"; }

 private:
  CQJavaScript*   js_ { 0 };
  QRadialGradient rg_;
};

#endif

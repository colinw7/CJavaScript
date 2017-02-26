#ifndef CQJCanvasRadialGradient_H
#define CQJCanvasRadialGradient_H

#include <CJObj.h>
#include <QRadialGradient>

class CQJCanvasRadialGradientType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJCanvasRadialGradientType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvasRadialGradient : public CJObj {
 public:
  CQJCanvasRadialGradient(CJavaScript *js, double x1=0, double y1=0, double r1=1,
                          double x2=0, double y2=0, double r2=1);

  CJValue *dup(CJavaScript *js) const override { return new CQJCanvasRadialGradient(js); }

  const QRadialGradient &rg() const { return rg_; }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(0); }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "canvasRadialGradient"; }

 private:
  QRadialGradient rg_;
};

#endif

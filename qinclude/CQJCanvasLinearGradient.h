#ifndef CQJCanvasLinearGradient_H
#define CQJCanvasLinearGradient_H

#include <CJObj.h>
#include <QLinearGradient>

class CQJCanvasLinearGradientType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJCanvasLinearGradientType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvasLinearGradient : public CJObj {
 public:
  CQJCanvasLinearGradient(CJavaScript *js, double x1=0, double y1=0, double x2=1, double y2=0);

  CJValue *dup(CJavaScript *js) const override { return new CQJCanvasLinearGradient(js); }

  const QLinearGradient &lg() const { return lg_; }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(0); }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "canvasLinearGradient"; }

 private:
  QLinearGradient lg_;
};

#endif

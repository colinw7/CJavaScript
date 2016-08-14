#ifndef CQJCanvasLinearGradient_H
#define CQJCanvasLinearGradient_H

#include <CJObj.h>
#include <QLinearGradient>

class CQJavaScript;

class CQJCanvasLinearGradientType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CQJCanvasLinearGradientType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvasLinearGradient : public CJObj {
 public:
  CQJCanvasLinearGradient(CQJavaScript *js, double x1=0, double y1=0, double x2=1, double y2=0);

  CQJavaScript *js() const { return js_; }

  CJValue *dup(CJavaScript *) const override { return new CQJCanvasLinearGradient(js_); }

  const QLinearGradient &lg() const { return lg_; }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "canvasLinearGradient"; }

 private:
  CQJavaScript*   js_ { 0 };
  QLinearGradient lg_;
};

#endif

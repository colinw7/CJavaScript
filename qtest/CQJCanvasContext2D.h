#ifndef CQJCanvasContext2D_H
#define CQJCanvasContext2D_H

#include <CJObj.h>

class CQJavaScript;

class CQJCanvasContext2DType : public CJObjType {
 public:
  static CJObjTypeP instance(CQJavaScript *js);

  CQJCanvasContext2DType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvasContext2D : public CJObj {
 public:
  CQJCanvasContext2D(CQJavaScript *js);

  CQJavaScript *js() const { return js_; }

  CJValue *dup(CJavaScript *) const override { return new CQJCanvasContext2D(js_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "canvasContext2D"; }

 private:
  void initFill();
  void initStroke();

 private:
  CQJavaScript *js_ { 0 };
};

#endif

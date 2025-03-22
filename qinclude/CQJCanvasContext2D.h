#ifndef CQJCanvasContext2D_H
#define CQJCanvasContext2D_H

#include <CJObj.h>

class CQJCanvasWidget;
class CQJCanvas;

class CQJCanvasContext2DType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJCanvasContext2DType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvasContext2D : public CJObj {
 public:
  CQJCanvasContext2D(CJavaScript *js, CQJCanvas *canvas);

  CJValue *dup(CJavaScript *js) const override { return new CQJCanvasContext2D(js, canvas_); }

  CQJCanvasWidget *canvasWidget() const;

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  OptReal toReal() const override { return OptReal(0); }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "canvasContext2D"; }

 private:
  void initFill();
  void initStroke();
  void initFont();

 private:
  CQJCanvas*  canvas_ { 0 };
  std::string lastFont_;
};

#endif

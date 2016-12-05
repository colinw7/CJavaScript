#ifndef CQJCanvasPattern_H
#define CQJCanvasPattern_H

#include <CJObj.h>
#include <QImage>

class CQJCanvasPatternType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CQJCanvasPatternType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvasPattern : public CJObj {
 public:
  enum class Repeat {
    None,
    XY,
    X,
    Y
  };

 public:
  CQJCanvasPattern(CJavaScript *js, const QImage &qimage, const Repeat &repeat);

  CJValue *dup(CJavaScript *js) const override {
    return new CQJCanvasPattern(js, qimage_, repeat_); }

  const QImage &qimage() const { return qimage_; }

  const Repeat &repeat() const { return repeat_; }
  void setRepeat(const Repeat &v) { repeat_ = v; }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(0); }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "canvasPattern"; }

 private:
  QImage qimage_;
  Repeat repeat_ { Repeat::XY };
};

#endif

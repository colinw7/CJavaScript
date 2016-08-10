#ifndef CQJCanvasPattern_H
#define CQJCanvasPattern_H

#include <CJObjectType.h>
#include <CJObject.h>

class CQJCanvasPatternType : public CJObjectType {
 public:
  static CJObjectTypeP instance();

  CQJCanvasPatternType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjectTypeP type_;
};

class CQJCanvasPattern : public CJObject {
 public:
  enum class Repeat {
    None,
    XY,
    X,
    Y
  };

 public:
  CQJCanvasPattern(CQJavaScript *js, const QImage &qimage, const Repeat &repeat);

  CQJavaScript *js() const { return js_; }

  CJValue *dup(CJavaScript *) const override {
    return new CQJCanvasPattern(js_, qimage_, repeat_); }

  const QImage &qimage() const { return qimage_; }

  const Repeat &repeat() const { return repeat_; }
  void setRepeat(const Repeat &v) { repeat_ = v; }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "canvasPattern"; }

 private:
  CQJavaScript* js_ { 0 };
  QImage        qimage_;
  Repeat        repeat_ { Repeat::XY };
};

#endif

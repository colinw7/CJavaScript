#ifndef CQJCanvas_H
#define CQJCanvas_H

#include <CJavaScript.h>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QFontMetrics>

class CQJavaScript;

class CQJCanvasType : public CJObjectType {
 public:
  CQJCanvasType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override;
};

class CQJCanvas : public CJObject {
 public:
  CQJCanvas(CQJavaScript *js);

  CQJavaScript *js() const { return js_; }

  CJValue *dup(CJavaScript *) const override { return new CQJCanvas(js_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  void updateSize();

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "canvas"; }

 private:
  CQJavaScript *js_ { 0 };
};

//------

class CQJCanvasContext2DType : public CJObjectType {
 public:
  CQJCanvasContext2DType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CQJCanvasContext2D : public CJObject {
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

//------

class CQJCanvasLinearGradientType : public CJObjectType {
 public:
  CQJCanvasLinearGradientType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CQJCanvasLinearGradient : public CJObject {
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

//------

class CQJCanvasRadialGradientType : public CJObjectType {
 public:
  CQJCanvasRadialGradientType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
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

//------

class CQJCanvasPatternType : public CJObjectType {
 public:
  CQJCanvasPatternType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
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

//------

class CQJCanvasFontMetricsType : public CJObjectType {
 public:
  CQJCanvasFontMetricsType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CQJCanvasFontMetrics : public CJObject {
 public:
  CQJCanvasFontMetrics(CQJavaScript *js, const QFont &font, const std::string &text);

  CQJavaScript *js() const { return js_; }

  CJValue *dup(CJavaScript *) const override {
    return new CQJCanvasFontMetrics(js_, font_, text_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "canvasFontMetrics"; }

 private:
  CQJavaScript* js_ { 0 };
  std::string   text_;
  QFont         font_;
  QFontMetricsF fm_;
};

#endif

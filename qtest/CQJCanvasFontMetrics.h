#ifndef CQJCanvasFontMetrics_H
#define CQJCanvasFontMetrics_H

#include <CJObj.h>
#include <CQJObject.h>
#include <QFont>
#include <QFontMetricsF>

class CQJCanvasFontMetricsType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CQJCanvasFontMetricsType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvasFontMetrics : public CQJObject {
  Q_OBJECT

 public:
  CQJCanvasFontMetrics(CQJavaScript *js, const QFont &font, const std::string &text);

  CQJavaScript *js() const { return js_; }

  CJValue *dup(CJavaScript *) const override { return new CQJCanvasFontMetrics(js_, font_, text_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "canvasFontMetrics"; }

 private:
  std::string   text_;
  QFont         font_;
  QFontMetricsF fm_;
};

#endif

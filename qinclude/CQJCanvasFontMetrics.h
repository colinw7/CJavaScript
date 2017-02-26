#ifndef CQJCanvasFontMetrics_H
#define CQJCanvasFontMetrics_H

#include <CJObj.h>
#include <CQJObject.h>
#include <QFont>
#include <QFontMetricsF>

class CQJCanvasFontMetricsType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJCanvasFontMetricsType(CJavaScript *js);

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
  CQJCanvasFontMetrics(CJavaScript *js, const QFont &font, const std::string &text);

  CJValue *dup(CJavaScript *js) const override {
    return new CQJCanvasFontMetrics(js, font_, text_);
  }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "canvasFontMetrics"; }

 private:
  std::string   text_;
  QFont         font_;
  QFontMetricsF fm_;
};

#endif

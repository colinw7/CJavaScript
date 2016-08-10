#ifndef CQJCanvas_H
#define CQJCanvas_H

#include <CQJObject.h>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QFontMetrics>

class CQJavaScript;

class CQJCanvasType : public CJObjectType {
 public:
  CQJCanvasType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override;
};

class CQJCanvas : public CQJObject {
  Q_OBJECT

 public:
  CQJCanvas(CQJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CQJCanvas(js_); }

  void updateSize();

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "canvas"; }
};

//------

#include <CQJCanvasContext2D.h>
#include <CQJCanvasLinearGradient.h>
#include <CQJCanvasRadialGradient.h>
#include <CQJCanvasPattern.h>
#include <CQJCanvasImageData.h>
#include <CQJCanvasImageDataArray.h>
#include <CQJCanvasFontMetrics.h>

#endif

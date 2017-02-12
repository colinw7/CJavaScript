#ifndef CQJCanvas_H
#define CQJCanvas_H

#include <CQJObject.h>

class CQJCanvasType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJCanvasType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override;

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvasWidget;

class CQJCanvas : public CQJObject {
  Q_OBJECT

 public:
  CQJCanvas(CJavaScript *js, CQJCanvasWidget *canvas);

  CJValue *dup(CJavaScript *js) const override { return new CQJCanvas(js, canvas_); }

  CQJCanvasWidget *canvas() const { return canvas_; }

  void updateSize();

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;

  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "canvas"; }

 private:
  CQJCanvasWidget* canvas_ { nullptr };
  CJValueP         context2D_;
};

#endif

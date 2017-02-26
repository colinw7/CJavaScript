#ifndef CQJCanvas_H
#define CQJCanvas_H

#include <CQJHtmlObj.h>

class CQJCanvasType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJCanvasType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override;

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvasIFace;
class CQJHtmlObjIFace;
class CQJCanvasWidget;

class CQJCanvas : public CQJHtmlObj {
  Q_OBJECT

 public:
  CQJCanvas(CJavaScript *js, CQJCanvasIFace *iface, CQJHtmlObjIFace *objIFace);

  CJValue *dup(CJavaScript *js) const override {
    return new CQJCanvas(js, iface_, CQJHtmlObj::iface_);
  }

  void setIFace(CQJCanvasIFace *iface) { iface_ = iface; }

  CQJCanvasWidget *canvasWidget() const;

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;

  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "canvas"; }

 private:
  CQJCanvasIFace* iface_ { nullptr };
  CJValueP        context2D_;
};

#endif

#ifndef CQJCanvas_H
#define CQJCanvas_H

#include <CQJObject.h>

class CQJavaScript;

class CQJCanvasType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CQJCanvasType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override;

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvas : public CQJObject {
  Q_OBJECT

 public:
  CQJCanvas(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CQJCanvas(js); }

  void updateSize();

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "canvas"; }

 private:
  CJValueP context2D_;
};

#endif

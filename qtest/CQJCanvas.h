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
  CQJCanvas(CQJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CQJCanvas(js_); }

  void updateSize();

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "canvas"; }
};

#endif

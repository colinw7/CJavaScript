#ifndef CQJInput_H
#define CQJInput_H

#include <CQJHtmlObj.h>
#include <CQJObject.h>

class CQJInputType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJInputType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJFormInputIFace;

class CQJInput : public CQJHtmlObj {
  Q_OBJECT

 public:
  CQJInput(CJavaScript *js, CQJFormInputIFace *iface, CQJHtmlObjIFace *objIFace);

  CJValue *dup(CJavaScript *js) const override {
    return new CQJInput(js, iface_, CQJHtmlObj::iface_);
  }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;

  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "input"; }

 protected:
  CQJFormInputIFace *iface_ { nullptr };
};

#endif

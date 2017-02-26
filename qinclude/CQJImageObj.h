#ifndef CQJImageObj_H
#define CQJImageObj_H

#include <CQJHtmlObj.h>
#include <CQJObject.h>

class CQJImageObjType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJImageObjType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJImageIFace;

class CQJImageObj : public CQJHtmlObj {
  Q_OBJECT

 public:
  CQJImageObj(CJavaScript *js, CQJImageIFace *iface, CQJHtmlObjIFace *objIFace);

  CJValue *dup(CJavaScript *js) const override {
    return new CQJImageObj(js, iface_, CQJHtmlObj::iface_);
  }

  QImage qimage() const;

  CJValueP getProperty(CJavaScript *js, const std::string &name) const;

  void setProperty(CJavaScript *js, const std::string &name, CJValueP value);

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "image"; }

 protected:
  CQJImageIFace *iface_ { nullptr };
};

#endif

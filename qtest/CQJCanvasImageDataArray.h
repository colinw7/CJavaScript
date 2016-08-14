#ifndef CQJCanvasImageDataArray_H
#define CQJCanvasImageDataArray_H

#include <CJObj.h>
#include <CQJObject.h>
#include <CQJCanvasImageData.h>

class CQJCanvasImageDataArrayType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CQJCanvasImageDataArrayType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvasImageDataArray : public CQJObject {
  Q_OBJECT

 public:
  CQJCanvasImageDataArray(CQJavaScript *js, CQJCanvasImageDataP data);

  CJValue *dup(CJavaScript *) const override { return new CQJCanvasImageDataArray(js_, data_); }

  const CQJCanvasImageDataP &data() const { return data_; }

  bool hasIndex() const override { return true; }

  CJValueP indexValue(int i) const override;

  void setIndexValue(int i, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "canvasImageDataArray"; }

 private:
  CQJCanvasImageDataP data_;
};

#endif

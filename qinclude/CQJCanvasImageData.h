#ifndef CQJCanvasImageData_H
#define CQJCanvasImageData_H

#include <CJObj.h>
#include <CQJObject.h>
#include <QImage>
#include <QColor>

class CQJCanvasImageDataType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJCanvasImageDataType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvasImageData : public CQJObject {
  Q_OBJECT

 public:
  CQJCanvasImageData(CJavaScript *js, int w, int h);
  CQJCanvasImageData(CJavaScript *js, const QImage &image);

  CJValue *dup(CJavaScript *js) const override { return new CQJCanvasImageData(js, qimage_); }

  const QImage &qimage() const { return qimage_; }

  QRgb getPixel(int x, int y) const { return qimage_.pixel(x, y); }
  void setPixel(int x, int y, const QRgb &rgb) { qimage_.setPixel(x, y, rgb); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "canvasImageData"; }

 private:
  QImage qimage_;
};

typedef std::shared_ptr<CQJCanvasImageData> CQJCanvasImageDataP;

#endif

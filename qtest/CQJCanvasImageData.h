#ifndef CQJCanvasImageData_H
#define CQJCanvasImageData_H

#include <CJObjectType.h>
#include <CQJObject.h>
#include <QImage>
#include <QColor>

class CQJCanvasImageDataType : public CJObjectType {
 public:
  static CJObjectTypeP instance();

  CQJCanvasImageDataType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjectTypeP type_;
};

class CQJCanvasImageData : public CQJObject {
  Q_OBJECT

 public:
  CQJCanvasImageData(CQJavaScript *js, int w, int h);
  CQJCanvasImageData(CQJavaScript *js, const QImage &image);

  CJValue *dup(CJavaScript *) const override { return new CQJCanvasImageData(js_, qimage_); }

  const QImage &qimage() const { return qimage_; }

  QRgb getPixel(int x, int y) const { return qimage_.pixel(x, y); }
  void setPixel(int x, int y, const QRgb &rgb) { qimage_.setPixel(x, y, rgb); }

  CJValueP getProperty(const std::string &key) const;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "canvasImageData"; }

 private:
  QImage qimage_;
};

typedef std::shared_ptr<CQJCanvasImageData> CQJCanvasImageDataP;

#endif

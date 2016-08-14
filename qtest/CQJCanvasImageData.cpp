#include <CQJCanvasImageData.h>
#include <CQJCanvasImageDataArray.h>
#include <CQJavaScript.h>

CJObjTypeP CQJCanvasImageDataType::type_;

CJObjTypeP
CQJCanvasImageDataType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CQJCanvasImageDataType(js));

  return type_;
}

CQJCanvasImageDataType::
CQJCanvasImageDataType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "CanvasImageData")
{
}

//------

CQJCanvasImageData::
CQJCanvasImageData(CQJavaScript *qjs, int w, int h) :
 CQJObject(qjs, CQJCanvasImageDataType::instance(qjs->js()))
{
  qimage_ = QImage(w, h, QImage::Format_ARGB32);

  CJavaScript *js = js_->js();

  setRealProperty(js, "width" , w);
  setRealProperty(js, "height", h);
}

CQJCanvasImageData::
CQJCanvasImageData(CQJavaScript *qjs, const QImage &image) :
 CQJObject(qjs, CQJCanvasImageDataType::instance(qjs->js())), qimage_(image)
{
  CJavaScript *js = js_->js();

  setRealProperty(js, "width" , image.width ());
  setRealProperty(js, "height", image.height());
}

CJValueP
CQJCanvasImageData::
getProperty(const std::string &key) const
{
  if (key == "data") {
    CQJCanvasImageData *th = const_cast<CQJCanvasImageData *>(this);

    CQJCanvasImageDataP data =
      std::static_pointer_cast<CQJCanvasImageData>(th->shared_from_this());

    return CJValueP(new CQJCanvasImageDataArray(js(), data));
  }
  else
    return CJObj::getProperty(key);
}

CJValueP
CQJCanvasImageData::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

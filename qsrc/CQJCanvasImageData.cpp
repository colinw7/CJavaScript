#include <CQJCanvasImageData.h>
#include <CQJCanvasImageDataArray.h>
#include <CJavaScript.h>

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
CQJCanvasImageData(CJavaScript *js, int w, int h) :
 CQJObject(js, CQJCanvasImageDataType::instance(js))
{
  qimage_ = QImage(w, h, QImage::Format_ARGB32);

  setRealProperty(js, "width" , w);
  setRealProperty(js, "height", h);
}

CQJCanvasImageData::
CQJCanvasImageData(CJavaScript *js, const QImage &image) :
 CQJObject(js, CQJCanvasImageDataType::instance(js)), qimage_(image)
{
  setRealProperty(js, "width" , image.width ());
  setRealProperty(js, "height", image.height());
}

CJValueP
CQJCanvasImageData::
getProperty(CJavaScript *js, const std::string &key) const
{
  if (key == "data") {
    CQJCanvasImageData *th = const_cast<CQJCanvasImageData *>(this);

    CQJCanvasImageDataP data = CJValue::cast<CQJCanvasImageData>(th->shared_from_this());

    return CJValueP(new CQJCanvasImageDataArray(js, data));
  }
  else
    return CJObj::getProperty(js, key);
}

CJValueP
CQJCanvasImageData::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

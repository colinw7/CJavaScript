#include <CQJCanvasImageDataArray.h>
#include <CJavaScript.h>

CJObjTypeP CQJCanvasImageDataArrayType::type_;

CJObjTypeP
CQJCanvasImageDataArrayType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CQJCanvasImageDataArrayType(js));

  return type_;
}

CQJCanvasImageDataArrayType::
CQJCanvasImageDataArrayType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "CanvasImageDataArray")
{
}

CQJCanvasImageDataArray::
CQJCanvasImageDataArray(CJavaScript *js, CQJCanvasImageDataP data) :
 CQJObject(js, CQJCanvasImageDataArrayType::instance(js)), data_(data)
{
  int w = data_->qimage().width ();
  int h = data_->qimage().height();

  setRealProperty(js, "length", 4*w*h);
}

CJValueP
CQJCanvasImageDataArray::
indexValue(long i) const
{
  int i1 = i / 4;
  int i0 = i % 4;

  int w = data_->qimage().width ();
  int h = data_->qimage().height();

  if (i1 < 0 || i1 >= w*h)
    //return CJValueP();
    return js_->createNumberValue(0L);

  int y = i1 / w;
  int x = i1 % w;

  QRgb rgb = data_->getPixel(x, y);

  long c = 0;

  if      (i0 == 0) c = qRed  (rgb);
  else if (i0 == 1) c = qGreen(rgb);
  else if (i0 == 2) c = qBlue (rgb);
  else if (i0 == 3) c = qAlpha(rgb);

  return js_->createNumberValue(c);
}

void
CQJCanvasImageDataArray::
setIndexValue(long i, CJValueP value)
{
  int i1 = i / 4;
  int i0 = i % 4;

  int w = data_->qimage().width ();
  int h = data_->qimage().height();

  if (i1 < 0 || i1 >= w*h)
    return;

  int y = i1 / w;
  int x = i1 % w;

  long c = value->toInteger().value_or(0L);

  QRgb rgb = data_->getPixel(x, y);

  QRgb rgb1;

  if      (i0 == 0) rgb1 = qRgba(c        , qGreen(rgb), qBlue(rgb), qAlpha(rgb));
  else if (i0 == 1) rgb1 = qRgba(qRed(rgb), c          , qBlue(rgb), qAlpha(rgb));
  else if (i0 == 2) rgb1 = qRgba(qRed(rgb), qGreen(rgb), c         , qAlpha(rgb));
  else if (i0 == 3) rgb1 = qRgba(qRed(rgb), qGreen(rgb), qBlue(rgb), c          );
  else assert(false);

  data_->setPixel(x, y, rgb1);

}

CJValueP
CQJCanvasImageDataArray::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

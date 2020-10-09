#include <CQJImageObj.h>
#include <CQJImageIFace.h>
#include <CQImageUtil.h>
#include <CQUtil.h>
#include <CJavaScript.h>
#include <CImageLib.h>

CJObjTypeP CQJImageObjType::type_;

CJObjTypeP
CQJImageObjType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJImageObjType(js));

    js->addObjectType("image", type_);
  }

  return type_;
}

CQJImageObjType::
CQJImageObjType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "ImageObj")
{
}

//------

CQJImageObj::
CQJImageObj(CJavaScript *js, CQJImageIFace *iface, CQJHtmlObjIFace *objIFace) :
 CQJHtmlObj(js, objIFace, CQJImageObjType::instance(js)), iface_(iface)
{
}

QImage
CQJImageObj::
qimage() const
{
  return CQImageUtil::toQImage(iface_->image());
}

CJValueP
CQJImageObj::
getProperty(CJavaScript *js, const std::string &name) const
{
  return CJObj::getProperty(js, name);
}

void
CQJImageObj::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJObj::setProperty(js, name, value);
}

CJValueP
CQJImageObj::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  return CQJObject::execNameFn(js, name, values);
}

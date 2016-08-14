#include <CQJImage.h>
#include <CQJavaScript.h>
#include <iostream>

CJObjTypeP CQJImageType::type_;

CJObjTypeP
CQJImageType::
instance(CQJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJImageType(js));

    js->js()->addObjectType("Image", type_);
  }

  return type_;
}

CQJImageType::
CQJImageType(CQJavaScript *js) :
 CJObjType(js->js(), CJToken::Type::Object, "Image"), js_(js)
{
}

CJValueP
CQJImageType::
construct(CJavaScript *, const Values &)
{
  return CJValueP(new CQJImage(js_));
}

//------

CQJImage::
CQJImage(CQJavaScript *qjs) :
 CQJObject(qjs, CQJImageType::instance(qjs))
{
  CJavaScript *js = js_->js();

  setStringProperty(js, "src"   , "");
  setStringProperty(js, "onLoad", "");
}

CJValueP
CQJImage::
getProperty(const std::string &name) const
{
  CJavaScript *js = js_->js();

  if      (name == "width") {
    return js->createNumberValue(long(qimage_.width()));
  }
  else if (name == "height") {
    return js->createNumberValue(long(qimage_.height()));
  }
  else
    return CQJObject::getProperty(name);
}

void
CQJImage::
setProperty(const std::string &name, CJValueP value)
{
  CJObj::setProperty(name, value);

  if      (name == "src") {
    std::string filename = value->toString();

    if (filename != "")
      qimage_ = QImage(filename.c_str());
  }
  else if (name == "onload") {
    CJavaScript *js = js_->js();

    if (value->type() == CJToken::Type::Function) {
      CJFunctionP fn = std::static_pointer_cast<CJFunction>(value);

      CJObjType::Values values;

      values.push_back(shared_from_this());

      fn->exec(js, values);
    }
  }
}

CJValueP
CQJImage::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  CJValueP obj = CQJObject::execNameFn(js, name, values);

  return obj;
}

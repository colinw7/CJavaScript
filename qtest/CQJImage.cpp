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
CQJImageType(CQJavaScript *qjs) :
 CJObjType(qjs->js(), CJToken::Type::Object, "Image"), qjs_(qjs)
{
}

CJValueP
CQJImageType::
construct(CJavaScript *, const Values &)
{
  return CJValueP(new CQJImage(qjs_));
}

//------

CQJImage::
CQJImage(CQJavaScript *qjs) :
 CQJObject(qjs, CQJImageType::instance(qjs))
{
  CJavaScript *js = qjs_->js();

  setStringProperty(js, "src"   , "");
  setStringProperty(js, "onLoad", "");
}

CJValueP
CQJImage::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "width") {
    return js->createNumberValue(long(qimage_.width()));
  }
  else if (name == "height") {
    return js->createNumberValue(long(qimage_.height()));
  }
  else
    return CQJObject::getProperty(js, name);
}

void
CQJImage::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJObj::setProperty(js, name, value);

  if      (name == "src") {
    std::string filename = value->toString();

    if (filename != "")
      qimage_ = QImage(filename.c_str());
  }
  else if (name == "onload") {
    if (value->type() == CJToken::Type::Function) {
      CJFunctionBaseP fn = CJValue::cast<CJFunctionBase>(value);

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

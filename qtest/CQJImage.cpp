#include <CQJImage.h>
#include <CQJavaScript.h>
#include <iostream>

CQJImageType::
CQJImageType(CQJavaScript *qjs) :
 CJObjectType(CJToken::Type::Object, "Image"), js_(qjs)
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
 CQJObject(qjs, qjs->jsImageType())
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
  CJObject::setProperty(name, value);

  if      (name == "src") {
    std::string filename = value->toString();

    if (filename != "")
      qimage_ = QImage(filename.c_str());
  }
  else if (name == "onload") {
    CJavaScript *js = js_->js();

    if (value->type() == CJToken::Type::Function) {
      CJFunctionP fn = std::static_pointer_cast<CJFunction>(value);

      CJObjectType::Values values;

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

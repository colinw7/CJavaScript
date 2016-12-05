#include <CQJImage.h>
#include <CJavaScript.h>
#include <iostream>

CJObjTypeP CQJImageType::type_;

CJObjTypeP
CQJImageType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJImageType(js));

    js->addObjectType("Image", type_);
  }

  return type_;
}

CQJImageType::
CQJImageType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Image")
{
}

CJValueP
CQJImageType::
construct(CJavaScript *js, const Values &)
{
  return CJValueP(new CQJImage(js));
}

//------

CQJImageFunction::
CQJImageFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "Image", CQJImageType::instance(js))
{
}

CQJImageFunction::
CQJImageFunction(const CQJImageFunction &fn) :
 CJObjTypeFunction(fn)
{
}

CJValueP
CQJImageFunction::
exec(CJavaScript *js, const Values &)
{
  // create image primitive
  CQJImageP image;

  image = CQJImageP(new CQJImage(js));

  return image;
}

CJValueP
CQJImageFunction::
execNew(CJavaScript *js, const Values &)
{
  // create image primitive
  CQJImageP image;

  image = CQJImageP(new CQJImage(js));

  return image;
}

std::string
CQJImageFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CQJImageFunction::
print(std::ostream &os) const
{
  os << "[Function: Image]";
}

//------

CQJImage::
CQJImage(CJavaScript *js) :
 CQJObject(js, CQJImageType::instance(js))
{
  setStringProperty(js, "src"   , "");
  setStringProperty(js, "onload", "");
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
      onLoad_ = CJValue::cast<CJFunctionBase>(value);

      CJObjType::Values values;

      values.push_back(shared_from_this());

      onLoad_->exec(js, values);
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

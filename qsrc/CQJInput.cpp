#include <CQJInput.h>
#include <CJavaScript.h>
#include <CQJFormInputIFace.h>

CJObjTypeP CQJInputType::type_;

CJObjTypeP
CQJInputType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJInputType(js));

    js->addObjectType("input", type_);
  }

  return type_;
}

CQJInputType::
CQJInputType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Input")
{
}

//------

CQJInput::
CQJInput(CJavaScript *js, CQJFormInputIFace *iface, CQJHtmlObjIFace *objIFace) :
 CQJHtmlObj(js, objIFace, CQJInputType::instance(js)), iface_(iface)
{
}

CJValueP
CQJInput::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "value") {
    return iface_->value(js);
  }
  else if (name == "valueAsNumber") {
    return iface_->numberValue(js);
  }

  return CJObj::getProperty(js, name);
}

void
CQJInput::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJObj::setProperty(js, name, value);
}

CJValueP
CQJInput::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  return CQJObject::execNameFn(js, name, values);
}

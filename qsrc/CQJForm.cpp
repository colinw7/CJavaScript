#include <CQJForm.h>
#include <CQJFormIFace.h>
#include <CQJHtmlCollection.h>
#include <CJavaScript.h>

CJObjTypeP CQJFormType::type_;

CJObjTypeP
CQJFormType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJFormType(js));

    js->addObjectType("form", type_);
  }

  return type_;
}

CQJFormType::
CQJFormType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Form")
{
}

//------

CQJForm::
CQJForm(CJavaScript *js, CQJFormIFace *iface, CQJHtmlObjIFace *objIFace) :
 CQJHtmlObj(js, objIFace, CQJFormType::instance(js)), iface_(iface)
{
}

CJValueP
CQJForm::
getProperty(CJavaScript *js, const std::string &name) const
{
  CQJHtmlCollection *col = new CQJHtmlCollection(js);

  for (const auto &input : iface_->inputs()) {
    if (input->getName() == name)
      col->addObject(input);
  }

  return CJValueP(col);
}

void
CQJForm::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJObj::setProperty(js, name, value);
}

CJValueP
CQJForm::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  return CQJObject::execNameFn(js, name, values);
}

#include <CQJHtmlObj.h>
#include <CQJHtmlObjIFace.h>
#include <CQJDocument.h>
#include <CQJavaScript.h>
#include <CJavaScript.h>

CJObjTypeP CQJHtmlObjType::type_;

CJObjTypeP
CQJHtmlObjType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJHtmlObjType(js));

    js->addObjectType("window", type_);
  }

  return type_;
}

CQJHtmlObjType::
CQJHtmlObjType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Html")
{
}

//------

CQJHtmlObj::
CQJHtmlObj(CJavaScript *js, CQJHtmlObjIFace *iface) :
 CQJObject(js, CQJHtmlObjType::instance(js)), iface_(iface)
{
  init();
}

CQJHtmlObj::
CQJHtmlObj(CJavaScript *js, CQJHtmlObjIFace *iface, const CJObjTypeP &type) :
 CQJObject(js, type), iface_(iface)
{
  init();
}

void
CQJHtmlObj::
init()
{
  objType_->addObjFunction(js_, "appendChild" , objType_);
  objType_->addObjFunction(js_, "insertBefore", objType_);
  objType_->addObjFunction(js_, "getAttribute", objType_);
}

std::string
CQJHtmlObj::
getName() const
{
  if (! iface_) return "";

  return iface_->getName();
}

std::string
CQJHtmlObj::
getClass() const
{
  if (! iface_) return "";

  return iface_->getClass();
}

std::string
CQJHtmlObj::
getType() const
{
  if (! iface_) return "";

  return iface_->getType();
}

std::string
CQJHtmlObj::
getId() const
{
  if (! iface_) return "";

  return iface_->getId();
}

CQJHtmlObj *
CQJHtmlObj::
parent() const
{
  if (! iface_) return nullptr;

  CQJHtmlObjIFace *parent = iface_->parent();
  if (! parent) return nullptr;

  return parent->obj();
}

CQJHtmlObj::Children
CQJHtmlObj::
children() const
{
  if (! iface_) return Children();

  return iface_->children();
}

CJValueP
CQJHtmlObj::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "parentNode") {
    CQJHtmlObj *parentObj = this->parent();
    if (! parentObj) return CJValueP();

    return CJValueP(parentObj);
  }
  else
    return CJObj::getProperty(js, name);
}

void
CQJHtmlObj::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJObj::setProperty(js, name, value);
}

CJValueP
CQJHtmlObj::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if      (name == "appendChild") {
    js->errorMsg("appendChild not implemented");
    return CJValueP();
  }
  else if (name == "insertBefore") {
    js->errorMsg("appendChild not implemented");
    return CJValueP();
  }
  else if (name == "getAttribute") {
    js->errorMsg("appendChild not implemented");
    return CJValueP();
  }
  else
    return CQJObject::execNameFn(js, name, values);
}

bool
CQJHtmlObj::
callEventListener(const std::string &name, const std::string &prop,
                  const EventArgs &args, const NameValues &nameValues)
{
  bool called = false;

  if (CQJObject::callEventListener(name, prop, args, nameValues))
    called = true;

  if (parent()) {
    if (parent()->callEventListener(name, prop, args, nameValues))
      called = true;
  }
  else {
    CQJDocumentP jsDocument = CQJavaScriptInst->jsDocument();

    if (jsDocument->callEventListener(name, prop, args, nameValues))
      called = true;
  }

  return called;
}

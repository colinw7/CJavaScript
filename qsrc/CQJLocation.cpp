#include <CQJLocation.h>
#include <CQJWindow.h>
#include <CJavaScript.h>
#include <CUrl.h>
#include <iostream>

CJObjTypeP CQJLocationType::type_;

CJObjTypeP
CQJLocationType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJLocationType(js));

    js->addObjectType("Location", type_);
  }

  return type_;
}

CQJLocationType::
CQJLocationType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Location")
{
}

CJValueP
CQJLocationType::
construct(CJavaScript *js, const Values &values)
{
  if (values.size() != 1)
    return CJValueP();

  CQJWindowP window = CJValue::cast<CQJWindow>(values[0]);

  return CJValueP(new CQJLocation(js, window.get()));
}

//------

CQJLocation::
CQJLocation(CJavaScript *js, CQJWindow *window) :
 CQJObject(js, CQJLocationType::instance(js)), window_(window)
{
}

CJValueP
CQJLocation::
getProperty(CJavaScript *js, const std::string &name) const
{
  const CUrl &url = window_->document()->url();

  if      (name == "href") {
    return js->createStringValue(url.getUrl());
  }
  else if (name == "protocol") {
    return js->createStringValue(url.getPrefix());
  }
  else if (name == "host") {
    return js->createStringValue(url.getSite());
  }
  else if (name == "hostname") {
    return js->createStringValue(url.getSite());
  }
  else if (name == "port") {
    return js->createStringValue("");
  }
  else if (name == "pathname") {
    return js->createStringValue(url.getFile());
  }
  else if (name == "search") {
    return js->createStringValue("");
  }
  else if (name == "hash") {
    return js->createStringValue(url.getTarget());
  }
  else if (name == "username") {
    return js->createStringValue("");
  }
  else if (name == "password") {
    return js->createStringValue("");
  }
  else if (name == "origin") {
    return js->createStringValue(url.getSite());
  }
  else
    return CJObj::getProperty(js, name);
}

void
CQJLocation::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJObj::setProperty(js, name, value);
}

CJValueP
CQJLocation::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  CJValueP obj = CQJObject::execNameFn(js, name, values);

  return obj;
}

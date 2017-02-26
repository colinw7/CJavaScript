#include <CQJDocument.h>
#include <CQJHtmlCollection.h>
#include <CQJHtmlObj.h>
#include <CQJDocumentIFace.h>
#include <CQJavaScript.h>
#include <CJavaScript.h>

CJObjTypeP CQJDocumentType::type_;

CJObjTypeP
CQJDocumentType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJDocumentType(js));

    js->addObjectType("document", type_);
  }

  return type_;
}

CQJDocumentType::
CQJDocumentType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Document")
{
}

//---

CQJDocument::
CQJDocument(CJavaScript *js, CQJWindowP window) :
 CQJObject(js, CQJDocumentType::instance(js)), window_(window)
{
  objType_->addObjFunction(js, "createElement"         , objType_);
  objType_->addObjFunction(js, "createTextNode"        , objType_);
  objType_->addObjFunction(js, "getElementById"        , objType_);
  objType_->addObjFunction(js, "getElementsByClassName", objType_);
  objType_->addObjFunction(js, "getElementsByTagName"  , objType_);
  objType_->addObjFunction(js, "querySelector"         , objType_);
}

CJValueP
CQJDocument::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "forms") {
    CQJHtmlCollection *col = new CQJHtmlCollection(js);

    std::vector<CQJHtmlObj *> objs;

    CQJavaScriptInst->getObjectsByTypeName("form", objs);
    if (objs.empty()) return CJValueP();

    for (auto &obj : objs)
      col->addObject(obj);

    return CJValueP(col);
  }
  else if (name == "head") {
    std::vector<CQJHtmlObj *> objs;

    CQJavaScriptInst->getObjectsByTypeName("head", objs);
    if (objs.empty()) return CJValueP();

    return objs[0]->shared_from_this();
  }
  else if (name == "body") {
    std::vector<CQJHtmlObj *> objs;

    CQJavaScriptInst->getObjectsByTypeName("body", objs);
    if (objs.empty()) return CJValueP();

    return objs[0]->shared_from_this();
  }
  else if (name == "URL") {
    return js->createStringValue(url_.getUrl());
  }
  else
    return CQJObject::getProperty(js, name);
}

CJValueP
CQJDocument::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if      (name == "createElement") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      if (! iface_)
        return CJValueP();

      return iface_->createElement(id);
    }

    return CJValueP();
  }
  else if (name == "createTextNode") {
    return CJValueP();
  }
  else if (name == "getElementById") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      CQJHtmlObj *obj = CQJavaScriptInst->getObjectById(id);
      if (! obj) return CJValueP();

      return obj->shared_from_this();
    }

    return CJValueP();
  }
  else if (name == "getElementsByClassName") {
    CQJHtmlCollection *col = new CQJHtmlCollection(js);

    if (values.size() == 2) {
      std::string name = values[1]->toString();

      std::vector<CQJHtmlObj *> objs;

      CQJavaScriptInst->getObjectsByClassName(name, objs);

      for (const auto &obj : objs)
        col->addObject(obj);
    }

    return CJValueP(col);
  }
  else if (name == "getElementsByTagName") {
    CQJHtmlCollection *col = new CQJHtmlCollection(js);

    if (values.size() == 2) {
      std::string name = values[1]->toString();

      std::vector<CQJHtmlObj *> objs;

      CQJavaScriptInst->getObjectsByTypeName(name, objs);

      for (const auto &obj : objs)
        col->addObject(obj);
    }

    return CJValueP(col);
  }
  else if (name == "querySelector") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      std::vector<CQJHtmlObj *> objs;

      CQJavaScriptInst->getObjectsBySelector(id, objs);

      if (objs.empty())
        return CJValueP();

      return objs[0]->shared_from_this();
    }

    return CJValueP();
  }

  return CQJObject::execNameFn(js, name, values);
}

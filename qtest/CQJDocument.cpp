#include <CQJDocument.h>
#include <CQJavaScript.h>

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
CQJDocument(CJavaScript *js) :
 CQJObject(js, CQJDocumentType::instance(js))
{
  objType_->addObjFunction(js, "createElement"         , objType_);
  objType_->addObjFunction(js, "createTextNode"        , objType_);
  objType_->addObjFunction(js, "getElementById"        , objType_);
  objType_->addObjFunction(js, "getElementsByClassName", objType_);
  objType_->addObjFunction(js, "querySelector"         , objType_);
}

CJValueP
CQJDocument::
getProperty(CJavaScript *js, const std::string &name) const
{
  return CQJObject::getProperty(js, name);
}

CJValueP
CQJDocument::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if      (name == "createElement") {
  }
  else if (name == "createTextNode") {
  }
  else if (name == "getElementById") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      if (id == "canvas") {
        CQJavaScript *qjs = CQJavaScript::instance();

        return qjs->jsCanvas();
      }
    }

    return CJValueP();
  }
  else if (name == "getElementsByClassName") {
  }
  else if (name == "querySelector") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      if (id == "canvas") {
        CQJavaScript *qjs = CQJavaScript::instance();

        return qjs->jsCanvas();
      }
    }

    return CJValueP();
  }

  return CQJObject::execNameFn(js, name, values);
}

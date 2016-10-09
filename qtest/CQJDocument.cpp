#include <CQJDocument.h>
#include <CQJavaScript.h>
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
CQJDocument(CQJavaScript *qjs) :
 CQJObject(qjs, CQJDocumentType::instance(qjs->js()))
{
  CJavaScript *js = qjs->js();

  objType_->addObjFunction(js, "getElementById", objType_);
  objType_->addObjFunction(js, "querySelector" , objType_);
}

CJValueP
CQJDocument::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if      (name == "getElementById") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      if (id == "canvas")
        return qjs_->jsCanvas();
    }

    return CJValueP();
  }
  else if (name == "querySelector") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      if (id == "canvas")
        return qjs_->jsCanvas();
    }

    return CJValueP();
  }

  return CQJObject::execNameFn(js, name, values);
}

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

  type_->addObjectFunction(js, "getElementById");
}

CJValueP
CQJDocument::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if (name == "getElementById") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      if (id == "canvas")
        return qjs_->jsCanvas();
    }

    return CJValueP();
  }

  return CQJObject::execNameFn(js, name, values);
}

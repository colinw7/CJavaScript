#include <CQJDocument.h>
#include <CQJavaScript.h>
#include <CQJavaScript.h>

CQJDocumentType::
CQJDocumentType() :
 CJObjectType(CJToken::Type::Object, "Document")
{
}

CQJDocument::
CQJDocument(CQJavaScript *qjs) :
 CJObject(qjs->jsDocumentType()), js_(qjs)
{
  CJavaScript *js = qjs->js();

  type_->addFunction(js, "getElementById");
}

CJValueP
CQJDocument::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if (name == "getElementById") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      if (id == "canvas")
        return js_->jsCanvas();
    }

    return CJValueP();
  }
  else
    return CJValueP();
}

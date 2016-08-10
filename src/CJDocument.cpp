#include <CJDocument.h>
#include <CJavaScript.h>

CJObjectTypeP CJDocumentType::type_;

CJObjectTypeP
CJDocumentType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CJDocumentType);

    js->addObjectType("document", type_);
  }

  return type_;
}

CJDocumentType::
CJDocumentType() :
 CJObjectType(CJToken::Type::Object, "Document")
{
}

CJValueP
CJDocumentType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//---

CJDocumentObject::
CJDocumentObject(CJavaScript *js) :
 CJObject(CJDocumentType::instance(js)), js_(js)
{
  type_->addFunction(js, "write");
}

CJValueP
CJDocumentObject::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if (name == "write") {
    int n = values.size();

    for (int i = 1; i < n; ++i) {
      if (i > 1) std::cout << " ";

      std::cout << *values[i];
    }

    std::cout << std::endl;
  }
  else
    js->errorMsg("Invalid document function - " + name);

  return CJValueP();
}

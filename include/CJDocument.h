#ifndef CJDocument_H
#define CJDocument_H

#include <CJObjectType.h>
#include <CJObject.h>

// Document Type
class CJDocumentType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CJDocumentType();

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjectTypeP type_;
};

// Document Object
class CJDocumentObject : public CJObject {
 public:
  CJDocumentObject(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJDocumentObject(js); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "document"; }

 private:
  CJavaScript *js_;
};

#endif

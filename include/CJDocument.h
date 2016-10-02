#ifndef CJDocument_H
#define CJDocument_H

#include <CJObj.h>

// Document Type
class CJDocumentType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJDocumentType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

// Document Object
class CJDocumentObject : public CJObj {
 public:
  CJDocumentObject(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJDocumentObject(js); }

  bool isHost() const override { return true; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif

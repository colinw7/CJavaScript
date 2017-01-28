#ifndef CQJDocument_H
#define CQJDocument_H

#include <CQJObject.h>

class CQJDocumentType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJDocumentType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJDocument : public CQJObject {
  Q_OBJECT

 public:
  explicit CQJDocument(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CQJDocument(js); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "document"; }
};

#endif

#ifndef CQJDocument_H
#define CQJDocument_H

#include <CQJObject.h>
#include <CJavaScript.h>

class CQJavaScript;

class CQJDocumentType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CQJDocumentType(CJavaScript *js);

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
  CQJDocument(CQJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CQJDocument(qjs_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "document"; }
};

#endif

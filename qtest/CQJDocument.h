#ifndef CQJDocument_H
#define CQJDocument_H

#include <CJavaScript.h>

class CQJavaScript;

class CQJDocumentType : public CJObjectType {
 public:
  CQJDocumentType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CQJDocument : public CJObject {
 public:
  CQJDocument(CQJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CQJDocument(js_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "document"; }

 private:
  CQJavaScript *js_ { 0 };
};

#endif

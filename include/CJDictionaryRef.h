#ifndef CJDictionaryRef_H
#define CJDictionaryRef_H

#include <CJLValue.h>

class CJDictionaryRef : public CJLValue {
 public:
  CJDictionaryRef(CJavaScript *js, CJDictionary *dict, const std::string &name);

  CJValue *dup(CJavaScript *js) const override { return new CJDictionaryRef(js, dict_, name_); }

  CJValueP value() const;

  void setValue(CJValueP value);

  std::string toString() const override { return ""; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const override {
    os << name_;
  }

 private:
  CJavaScript*  js_ { 0 };
  CJDictionary* dict_ { 0 };
  std::string   name_;
};

#endif

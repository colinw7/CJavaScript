#ifndef CJDictionaryValue_H
#define CJDictionaryValue_H

#include <CJLValue.h>

class CJDictionaryValue : public CJLValue {
 public:
  CJDictionaryValue(CJavaScript *js, CJDictionaryP dict, const std::string &name);

  CJValue *dup(CJavaScript *js) const override { return new CJDictionaryValue(js, dict_, name_); }

  std::string toString() const override;

  double toReal() const override;

  bool toBoolean() const override;

  CJValueP value() const;

  void setValue(CJValueP value);

  void print(std::ostream &os) const;

 private:
  CJavaScript*  js_;
  CJDictionaryP dict_;
  std::string   name_;
};

#endif

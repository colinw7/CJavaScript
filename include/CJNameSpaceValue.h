#ifndef CJNameSpaceValue_H
#define CJNameSpaceValue_H

#include <CJLValue.h>

class CJNameSpaceValue : public CJLValue {
 public:
  CJNameSpaceValue(CJavaScript *js, CJNameSpaceP scope, const std::string &name);

  CJValue *dup(CJavaScript *js) const override { return new CJNameSpaceValue(js, scope_, name_); }

  std::string toString() const override;

  double toReal() const override;

  bool toBoolean() const override;

  // get/set value
  CJValueP value() const override;
  void setValue(CJValueP value) override;

  bool isReadOnly() const override;
  void setReadOnly(bool b) override;

  void print(std::ostream &os) const override;

 private:
  CJavaScript* js_ { 0 };
  CJNameSpaceP scope_;
  std::string  name_;
};

#endif

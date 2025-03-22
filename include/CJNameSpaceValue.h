#ifndef CJNameSpaceValue_H
#define CJNameSpaceValue_H

#include <CJLValue.h>

class CJNameSpaceValue : public CJLValue {
 public:
  CJNameSpaceValue(CJavaScript *js, CJNameSpaceP scope, const std::string &name);

  CJNameSpaceValue(const CJNameSpaceValue &value);

  CJValue *dup(CJavaScript *) const override { return new CJNameSpaceValue(*this); }

  //---

  OptReal toReal() const override;

  OptLong toInteger() const override;

  bool toBoolean() const override;

  //---

  // get/set value
  CJValueP value() const override;
  void setValue(CJValueP value) override;

  bool isWritable() const override;
  void setWritable(bool b) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJavaScript* js_ { 0 };
  CJNameSpaceP scope_;
  std::string  name_;
};

#endif

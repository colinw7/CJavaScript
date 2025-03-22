#ifndef CJTypeValue_H
#define CJTypeValue_H

#include <CJLValue.h>
#include <vector>

class CJTypeValue : public CJLValue {
 public:
  typedef std::vector<CJValueP> Values;

 public:
  CJTypeValue(CJavaScript *js, CJObjTypeP objType, const std::string &name);

  CJTypeValue(const CJTypeValue &value);

  CJValue *dup(CJavaScript *) const override { return new CJTypeValue(*this); }

  CJObjTypeP getObjType() const { return objType_; }

  // get/set value
  CJValueP value() const override;
  void setValue(CJValueP value) override;

  bool hasValue() const;

  //---

  OptReal toReal() const override { return (hasValue() ? value()->toReal() : OptReal()); }

  OptLong toInteger() const override { return (hasValue() ? value()->toInteger() : OptLong()); }

  bool toBoolean() const override { return hasValue() ? value()->toBoolean() : false; }

  //---

  std::string toString() const override { return hasValue() ? value()->toString() : ""; }

  void print(std::ostream &os) const override;

 private:
  CJavaScript* js_ { 0 };
  CJObjTypeP   objType_;
  std::string  name_;
};

#endif

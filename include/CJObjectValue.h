#ifndef CJObjectValue_H
#define CJObjectValue_H

#include <CJLValue.h>
#include <CJTypes.h>
#include <vector>

class CJObjectValue : public CJLValue {
 public:
  typedef std::vector<CJValueP> Values;

 public:
  CJObjectValue(CJavaScript *js, CJObjP obj, const std::string &name);

  CJValue *dup(CJavaScript *js) const override { return new CJObjectValue(js, obj_, name_); }

  CJObjP getObject() const { return obj_; }

  // get/set value
  CJValueP value() const override;
  void setValue(CJValueP value) override;

  //---

  COptReal toReal() const override { return value()->toReal(); }

  COptLong toInteger() const override { return value()->toInteger(); }

  bool toBoolean() const override { return value()->toBoolean(); }

  //---

  std::string toString() const override { return value()->toString(); }

  void print(std::ostream &os) const override;

 private:
  CJavaScript* js_ { 0 };
  CJObjP       obj_;
  std::string  name_;
};

#endif

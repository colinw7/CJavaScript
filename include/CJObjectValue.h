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

  CJValueP value() const;

  void setValue(CJValueP value);

  std::string toString() const override { return value()->toString(); }

  double toReal() const override { return value()->toReal(); }

  bool toBoolean() const override { return value()->toBoolean(); }

  void print(std::ostream &os) const override;

 private:
  CJavaScript* js_ { 0 };
  CJObjP       obj_;
  std::string  name_;
};

#endif

#ifndef CJTypeValue_H
#define CJTypeValue_H

#include <CJLValue.h>
#include <vector>

class CJTypeValue : public CJLValue {
 public:
  typedef std::vector<CJValueP> Values;

 public:
  CJTypeValue(CJavaScript *js, CJObjTypeP objType, const std::string &name);

  CJValue *dup(CJavaScript *js) const override { return new CJTypeValue(js, objType_, name_); }

  CJObjTypeP getObjType() const { return objType_; }

  CJValueP value() const;

  void setValue(CJValueP value);

  bool hasValue() const;

  std::string toString() const override { return hasValue() ? value()->toString() : ""; }

  double toReal() const override { return hasValue() ? value()->toReal() : 0.0; }

  bool toBoolean() const override { return hasValue() ? value()->toBoolean() : false; }

  void print(std::ostream &os) const override;

 private:
  CJavaScript*  js_ { 0 };
  CJObjTypeP objType_;
  std::string   name_;
};

#endif

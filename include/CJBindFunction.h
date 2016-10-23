#ifndef CJBindFunction_H
#define CJBindFunction_H

#include <CJFunctionBase.h>

// function called on another function
class CJBindFunction : public CJFunctionBase {
 public:
  CJBindFunction(CJavaScript *js, CJFunctionBaseP function,
                 CJValueP thisValue, const Values &values);

  CJValue *dup(CJavaScript *js) const override {
    return new CJBindFunction(js, function_, thisValue_, values_);
  }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJFunctionBaseP function_;
  CJValueP        thisValue_;
  Values          values_;
};

#endif

#ifndef CJCallFunction_H
#define CJCallFunction_H

#include <CJFunctionBase.h>

// function called on another function
class CJCallFunction : public CJFunctionBase {
 public:
  CJCallFunction(CJavaScript *js, CJFunctionBaseP function);

  CJValue *dup(CJavaScript *js) const override { return new CJCallFunction(js, function_); }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;
  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "call " << function_->name();
  }

 private:
  CJFunctionBaseP function_;
};

#endif

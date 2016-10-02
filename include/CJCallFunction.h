#ifndef CJCallFunction_H
#define CJCallFunction_H

#include <CJFunctionBase.h>

// function called on another function
class CJCallFunction : public CJFunctionBase {
 public:
  enum class Type {
    Bind,
    Call,
    Apply
  };

 public:
  CJCallFunction(CJavaScript *js, CJFunctionBaseP function, Type type, CJObjTypeP objType);

  CJValue *dup(CJavaScript *js) const override {
    return new CJCallFunction(js, function_, type_, objType_);
  }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;
  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJFunctionBaseP function_;
  Type            type_;
  CJObjTypeP      objType_;
};

#endif

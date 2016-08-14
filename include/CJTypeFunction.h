#ifndef CJTypeFunction_H
#define CJTypeFunction_H

#include <CJFunction.h>

// function called on type name
class CJTypeFunction : public CJFunction {
 public:
  CJTypeFunction(CJavaScript *js, const std::string &name, CJObjTypeP type=CJObjTypeP());

  CJObjTypeP objectType() const { return type_; }
  void setObjectType(CJObjTypeP type) { type_ = type; }

  CJValue *dup(CJavaScript *js) const override { return new CJTypeFunction(js, name_, type_); }

  bool hasObjectValue() const override { return false; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "[Function " + name_ + "]";
  }

 private:
  CJObjTypeP type_;
};

typedef std::shared_ptr<CJTypeFunction> CJTypeFunctionP;

#endif

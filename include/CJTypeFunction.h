#ifndef CJTypeFunction_H
#define CJTypeFunction_H

#include <CJFunction.h>

// type function
class CJTypeFunction : public CJFunction {
 public:
  CJTypeFunction(CJavaScript *js, const std::string &name, CJObjectTypeP type=CJObjectTypeP()) :
   CJFunction(js, name, CJFunction::Type::Object), type_(type) {
  }

  CJObjectTypeP objectType() const { return type_; }
  void setObjectType(CJObjectTypeP type) { type_ = type; }

  CJValue *dup(CJavaScript *js) const override { return new CJTypeFunction(js, name_, type_); }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "type fn";
  }

 private:
  CJObjectTypeP type_;
};

typedef std::shared_ptr<CJTypeFunction> CJTypeFunctionP;

#endif

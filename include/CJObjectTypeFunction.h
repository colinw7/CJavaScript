#ifndef CJObjectTypeFunction_H
#define CJObjectTypeFunction_H

#include <CJFunctionBase.h>

// function called on object but shared across types
class CJObjectTypeFunction : public CJFunctionBase {
 public:
  CJObjectTypeFunction(CJavaScript *js, const std::string &name, CJObjTypeP type=CJObjTypeP());

  CJObjectTypeFunction(const CJObjectTypeFunction &fn);

  CJObjTypeP objectType() const { return type_; }
  void setObjectType(CJObjTypeP type) { type_ = type; }

  CJValue *dup(CJavaScript *) const override { return new CJObjectTypeFunction(*this); }

  CJObjTypeP type() const { return type_; }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJObjTypeP type_;
};

typedef std::shared_ptr<CJObjectTypeFunction> CJObjectTypeFunctionP;

#endif

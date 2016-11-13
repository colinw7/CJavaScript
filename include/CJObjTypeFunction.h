#ifndef CJObjTypeFunction_H
#define CJObjTypeFunction_H

#include <CJFunctionBase.h>

// function called on type name
class CJObjTypeFunction : public CJFunctionBase {
 public:
  CJObjTypeFunction(CJavaScript *js, const std::string &name, CJObjTypeP type=CJObjTypeP());

  CJObjTypeFunction(const CJObjTypeFunction &fn);

  CJObjTypeP objectType() const { return type_; }
  void setObjectType(CJObjTypeP type) { type_ = type; }

  CJValue *dup(CJavaScript *) const override { return new CJObjTypeFunction(*this); }

  bool hasObjectValue() const override { return true; }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;
  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP exec(CJavaScript *js, const Values &values) override;

  virtual CJValueP execNew(CJavaScript *js, const Values &values);

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJObjTypeP type_;
};

#endif

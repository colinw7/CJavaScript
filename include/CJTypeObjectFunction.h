#ifndef CJTypeObjectFunction_H
#define CJTypeObjectFunction_H

class CJTypeObjectFunction : public CJFunction {
 public:
  CJTypeObjectFunction(CJavaScript *js, CJObjectTypeP obj, const std::string &name) :
   CJFunction(js, name, Type::Normal), obj_(obj), name_(name) {
  }

  CJValue *dup(CJavaScript *js) const override {
    return new CJTypeObjectFunction(js, obj_, name_);
  }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) {
    return obj_->exec(js, name_, values);
  }

 private:
  CJObjectTypeP obj_;
  std::string   name_;
};

typedef std::shared_ptr<CJTypeObjectFunction> CJTypeObjectFunctionP;

#endif

#ifndef CJFunction_H
#define CJFunction_H

#include <CJObjectType.h>
#include <CJValue.h>
#include <vector>

// Function Type
class CJFunctionType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CJFunctionType();

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjectTypeP type_;
};

//------

// Function Value
class CJFunction : public CJValue {
 public:
  enum class Type {
    Normal,
    Real,
    Real2,
    Random,
    User,
    Object
  };

  typedef std::vector<CJValueP> Values;

 public:
  CJFunction(CJavaScript *js, const std::string &name, Type type=Type::Normal);

  CJValue *dup(CJavaScript *) const override { assert(false); return 0; }

  bool isProtoValue() const override { return true; }

  const std::string &name() const { return name_; }

  const Type &type() const { return type_; }

  std::string toString() const override { return name_; }

  double toReal() const override { return 1; }

  bool toBoolean() const override { return true; }

  virtual bool hasObjectValue() const { return false; }

  virtual CJValueP exec(CJavaScript *js, const Values &values) = 0;

  void print(std::ostream &os) const override {
    os << name_ << "()";
  }

 protected:
  std::string name_;
  Type        type_;
};

#endif

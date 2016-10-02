#ifndef CJRealFunction_H
#define CJRealFunction_H

#include <CJFunctionBase.h>

// function calling API with signature: double result = fn(double r);
class CJMathFunction : public CJFunctionBase {
 public:
  typedef double (*Fn)(double);

 public:
  CJMathFunction(CJavaScript *js, const std::string &name, Fn fn=0) :
   CJFunctionBase(js, name, CJFunctionBase::Type::Real), fn_(fn) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJMathFunction(js, name_, fn_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 protected:
  Fn fn_;
};

// function calling API with signature: double result = fn(double r1, double r2);
class CJReal2Function : public CJFunctionBase {
 public:
  typedef double (*Fn)(double, double);

 public:
  CJReal2Function(CJavaScript *js, const std::string &name, Fn fn) :
   CJFunctionBase(js, name, CJFunctionBase::Type::Real2), fn_(fn) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJReal2Function(js, name_, fn_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 protected:
  Fn fn_;
};

// min of values
class CJMinFunction : public CJFunctionBase {
 public:
  CJMinFunction(CJavaScript *js) :
   CJFunctionBase(js, "min", CJFunctionBase::Type::Min) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJMinFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

// min of values
class CJMaxFunction : public CJFunctionBase {
 public:
  CJMaxFunction(CJavaScript *js) :
   CJFunctionBase(js, "max", CJFunctionBase::Type::Max) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJMaxFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif

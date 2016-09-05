#ifndef CJRealFunction_H
#define CJRealFunction_H

#include <CJFunction.h>

// function calling API with signature: double result = fn(double r);
class CJRealFunction : public CJFunction {
 public:
  typedef double (*Fn)(double);

 public:
  CJRealFunction(CJavaScript *js, const std::string &name, Fn fn) :
   CJFunction(js, name, CJFunction::Type::Real), fn_(fn) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJRealFunction(js, name_, fn_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "double " << name_ << "(double)";
  }

 protected:
  Fn fn_;
};

// function calling API with signature: double result = fn(double r1, double r2);
class CJReal2Function : public CJFunction {
 public:
  typedef double (*Fn)(double, double);

 public:
  CJReal2Function(CJavaScript *js, const std::string &name, Fn fn) :
   CJFunction(js, name, CJFunction::Type::Real2), fn_(fn) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJReal2Function(js, name_, fn_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "double " << name_ << "(double, double)";
  }

 protected:
  Fn fn_;
};

// min of values
class CJMinFunction : public CJFunction {
 public:
  CJMinFunction(CJavaScript *js) :
   CJFunction(js, "min", CJFunction::Type::Min) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJMinFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "double min(...)";
  }
};

// min of values
class CJMaxFunction : public CJFunction {
 public:
  CJMaxFunction(CJavaScript *js) :
   CJFunction(js, "max", CJFunction::Type::Max) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJMaxFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "double max(...)";
  }
};

#endif

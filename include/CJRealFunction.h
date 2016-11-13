#ifndef CJRealFunction_H
#define CJRealFunction_H

#include <CJFunctionBase.h>

// function calling API with signature: double result = fn(double r);
class CJMathFunction : public CJFunctionBase {
 public:
  CJMathFunction(CJavaScript *js, const std::string &name, CJRealFn1 fn=0);

  CJMathFunction(const CJMathFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJMathFunction(*this); }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 protected:
  CJRealFn1 fn_;
};

// function calling API with signature: double result = fn(double r1, double r2);
class CJReal2Function : public CJFunctionBase {
 public:
  CJReal2Function(CJavaScript *js, const std::string &name, CJRealFn2 fn);

  CJReal2Function(const CJReal2Function &fn);

  CJValue *dup(CJavaScript *) const override { return new CJReal2Function(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 protected:
  CJRealFn2 fn_;
};

// min of values
class CJMinFunction : public CJFunctionBase {
 public:
  CJMinFunction(CJavaScript *js);

  CJMinFunction(const CJMinFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJMinFunction(*this); }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

// min of values
class CJMaxFunction : public CJFunctionBase {
 public:
  CJMaxFunction(CJavaScript *js);

  CJMaxFunction(const CJMaxFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJMaxFunction(*this); }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif

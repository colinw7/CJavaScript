#ifndef CJErrorFunction_H
#define CJErrorFunction_H

#include <CJObjTypeFunction.h>

// base error function
class CJErrorFunctionBase : public CJObjTypeFunction {
 public:
  CJErrorFunctionBase(CJavaScript *js, const std::string &name, CJObjTypeP type);

  CJValueP execNew(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

//------

// error function
class CJErrorFunction : public CJErrorFunctionBase {
 public:
  CJErrorFunction(CJavaScript *js);

  CJErrorFunction(const CJErrorFunction &error);

  CJValue *dup(CJavaScript *) const override { return new CJErrorFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// type error function
class CJTypeErrorFunction : public CJErrorFunctionBase {
 public:
  CJTypeErrorFunction(CJavaScript *js);

  CJTypeErrorFunction(const CJTypeErrorFunction &error);

  CJValue *dup(CJavaScript *) const override { return new CJTypeErrorFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// reference error function
class CJReferenceErrorFunction : public CJErrorFunctionBase {
 public:
  CJReferenceErrorFunction(CJavaScript *js);

  CJReferenceErrorFunction(const CJReferenceErrorFunction &error);

  CJValue *dup(CJavaScript *) const override { return new CJReferenceErrorFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// eval error function
class CJEvalErrorFunction : public CJErrorFunctionBase {
 public:
  CJEvalErrorFunction(CJavaScript *js);

  CJEvalErrorFunction(const CJEvalErrorFunction &error);

  CJValue *dup(CJavaScript *) const override { return new CJEvalErrorFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// range error function
class CJRangeErrorFunction : public CJErrorFunctionBase {
 public:
  CJRangeErrorFunction(CJavaScript *js);

  CJRangeErrorFunction(const CJRangeErrorFunction &error);

  CJValue *dup(CJavaScript *) const override { return new CJRangeErrorFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// syntax error function
class CJSyntaxErrorFunction : public CJErrorFunctionBase {
 public:
  CJSyntaxErrorFunction(CJavaScript *js);

  CJSyntaxErrorFunction(const CJSyntaxErrorFunction &error);

  CJValue *dup(CJavaScript *) const override { return new CJSyntaxErrorFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// uri error function
class CJURIErrorFunction : public CJErrorFunctionBase {
 public:
  CJURIErrorFunction(CJavaScript *js);

  CJURIErrorFunction(const CJURIErrorFunction &error);

  CJValue *dup(CJavaScript *) const override { return new CJURIErrorFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

#endif

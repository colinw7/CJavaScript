#ifndef CJErrorFunction_H
#define CJErrorFunction_H

#include <CJTypeFunction.h>

// base error function
class CJErrorFunctionBase : public CJTypeFunction {
 public:
  CJErrorFunctionBase(CJavaScript *js, const std::string &name, CJObjTypeP type);

  void print(std::ostream &os) const override {
    os << "[Function: " + name_ + "]";
  }
};

//------

// error function
class CJErrorFunction : public CJErrorFunctionBase {
 public:
  CJErrorFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJErrorFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// type error function
class CJTypeErrorFunction : public CJErrorFunctionBase {
 public:
  CJTypeErrorFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJTypeErrorFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// reference error function
class CJReferenceErrorFunction : public CJErrorFunctionBase {
 public:
  CJReferenceErrorFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJReferenceErrorFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// eval error function
class CJEvalErrorFunction : public CJErrorFunctionBase {
 public:
  CJEvalErrorFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJEvalErrorFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// range error function
class CJRangeErrorFunction : public CJErrorFunctionBase {
 public:
  CJRangeErrorFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJRangeErrorFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// syntax error function
class CJSyntaxErrorFunction : public CJErrorFunctionBase {
 public:
  CJSyntaxErrorFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJSyntaxErrorFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// uri error function
class CJURIErrorFunction : public CJErrorFunctionBase {
 public:
  CJURIErrorFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJURIErrorFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

#endif

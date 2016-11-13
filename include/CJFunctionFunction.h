#ifndef CJFunctionFunction_H
#define CJFunctionFunction_H

#include <CJObjTypeFunction.h>

// Function constructor function
class CJFunctionFunction : public CJObjTypeFunction {
 public:
  CJFunctionFunction(CJavaScript *js);

  CJFunctionFunction(const CJFunctionFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJFunctionFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  CJValueP execNew(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  bool expandArg(const std::string &arg, std::vector<std::string> &args) const;
};

typedef std::shared_ptr<CJFunctionFunction> CJFunctionFunctionP;

#endif

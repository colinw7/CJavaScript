#ifndef CJFunctionFunction_H
#define CJFunctionFunction_H

#include <CJTypeFunction.h>

// Function constructor function
class CJFunctionFunction : public CJTypeFunction {
 public:
  CJFunctionFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJFunctionFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "[Function: Object]";
  }

 private:
  bool expandArg(const std::string &arg, std::vector<std::string> &args) const;
};

typedef std::shared_ptr<CJFunctionFunction> CJFunctionFunctionP;

#endif

#ifndef CJExecFunction_H
#define CJExecFunction_H

#include <CJToken.h>
#include <CJTypes.h>
#include <vector>
#include <iostream>

// function <name> ( <args> ) { [<statement>]* }
class CJExecFunction : public CJToken {
 public:
  CJExecFunction();

  void setIdentifiers(CJExecIdentifiersP identifiers) {
    identifiers_ = identifiers;
  }

  void setExpression(CJExecExpressionP expr) {
    expr_ = expr;
  }

  void setExprList(CJExecExpressionListP exprList) {
    exprList_ = exprList;
  }

  void setEValue(CJValueP evalue) {
    evalue_ = evalue;
  }

  void addFunction(CJExecFunctionP fn) {
    functions_.push_back(fn);
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  typedef std::vector<CJExecFunctionP> Functions;

  CJExecIdentifiersP    identifiers_;
  CJExecExpressionP     expr_;
  CJExecExpressionListP exprList_;
  CJValueP              evalue_;
  Functions             functions_;
};

#endif

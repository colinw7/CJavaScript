#ifndef CJExecFor_H
#define CJExecFor_H

#include <CJToken.h>
#include <CJTypes.h>
#include <iostream>

// for (<expression_list> ; <expression_list> ; <expression_list> ) <block>
class CJExecFor : public CJToken {
 public:
  CJExecFor();

  void setExprList1(CJExecExpressionListP exprList) {
    exprList1_ = exprList;
  }

  void setVar(CJExecVarP var) {
    var_ = var;
  }

  void setExprList2(CJExecExpressionListP exprList) {
    exprList2_ = exprList;
  }

  void setExprList3(CJExecExpressionListP exprList) {
    exprList3_ = exprList;
  }

  void setIdentifiers(CJExecIdentifiersP identifiers, bool var) {
    identifiers_ = identifiers;
    ivar_        = var;
  }

  void setInExpr(CJExecExpressionP expr) {
    inExpr_ = expr;
  }

  void setBlock(CJExecBlockP block) {
    block_ = block;
  }

  void interp(CJavaScript *js);

  CJValueP exec(CJavaScript *js) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJExecVarP            var_;
  CJExecExpressionListP exprList1_;
  CJExecExpressionListP exprList2_;
  CJExecExpressionListP exprList3_;
  CJExecExpressionP     inExpr_;
  CJExecIdentifiersP    identifiers_;
  bool                  ivar_ { false };
  CJExecBlockP          block_;
};

#endif

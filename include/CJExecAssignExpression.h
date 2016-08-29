#ifndef CJExecAssignExpression_H
#define CJExecAssignExpression_H

#include <CJExecExpression.h>
#include <iostream>

// <identifiers> <assign_op> <expression>
class CJExecAssignExpression : public CJExecExpression {
 public:
  CJExecAssignExpression();

  void setLExpression(CJExecExpressionP lexpr) {
    lexpr_ = lexpr;
  }

  void setOperator(CJOperatorP op) {
    op_ = op;
  }

  void setRExpression(CJExecExpressionP rexpr) {
    rexpr_ = rexpr;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  CJExecExpressionP lexpr_;
  CJOperatorP       op_;
  CJExecExpressionP rexpr_;
};

#endif

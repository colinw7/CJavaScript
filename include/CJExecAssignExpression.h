#ifndef CJExecAssignExpression_H
#define CJExecAssignExpression_H

#include <CJExecExpression.h>
#include <iostream>

// <identifiers> <assign_op> <expression>
class CJExecAssignExpression : public CJExecExpression {
 public:
  typedef std::vector<CJValueP> Values;

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

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJValueP getIndexValue(CJavaScript *js, CJValueP varValue, const Values &ivalues);
  bool     setIndexValue(CJavaScript *js, CJValueP varValue, const Values &ivalues, CJValueP value);

 private:
  CJExecExpressionP lexpr_;
  CJOperatorP       op_;
  CJExecExpressionP rexpr_;
};

#endif

#ifndef CJExecIncrDecrExpression_H
#define CJExecIncrDecrExpression_H

#include <CJExecExpression.h>
#include <iostream>

// --<identifiers> | <identifiers>-- | ++<identifiers> | <identifiers>++
class CJExecIncrDecrExpression : public CJExecExpression {
 public:
  CJExecIncrDecrExpression();

  void setIdentifiers(CJExecIdentifiersP identifiers) {
    identifiers_ = identifiers;
  }

  void setExpression(CJExecExpressionP expr) {
    expr_ = expr;
  }

  void setOperator(CJOperatorP op) {
    op_ = op;
  }

  void setPost(bool b) {
    post_ = b;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  CJExecIdentifiersP identifiers_;
  CJExecExpressionP  expr_;
  CJOperatorP        op_;
  bool               post_ { false };
};

#endif

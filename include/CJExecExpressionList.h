#ifndef CJExecExpressionList_H
#define CJExecExpressionList_H

#include <CJToken.h>
#include <vector>
#include <iostream>

// <expression> [, <expression>]*
class CJExecExpressionList : public CJToken {
 public:
  typedef std::vector<CJExecExpressionP> Expressions;
  typedef std::vector<CJValueP>          Values;

 public:
  explicit CJExecExpressionList(CJToken::Type type=CJToken::Type::ExpressionList);

  const Expressions &expressions() const { return expressions_; }

  void addExpression(CJExecExpressionP expr);

  CJExecExpressionP indexExpression(int i);

  Values getValues(CJavaScript *js) const;

  bool isEmpty() const;

  CJValueP exec(CJavaScript *js) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  Expressions expressions_;
};

#endif

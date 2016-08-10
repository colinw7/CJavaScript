#ifndef CJExecExpressionList_H
#define CJExecExpressionList_H

#include <CJToken.h>
#include <iostream>

// <expression> [, <expression>]*
class CJExecExpressionList : public CJToken {
 public:
  typedef std::vector<CJExecExpressionP> Expressions;
  typedef std::vector<CJValueP>          Values;

 public:
  CJExecExpressionList(CJToken::Type type=CJToken::Type::ExpressionList) :
   CJToken(type) {
  }

  const Expressions &expressions() const { return expressions_; }

  void addExpression(CJExecExpressionP expr) {
    expressions_.push_back(expr);
  }

  CJExecExpressionP indexExpression(int i) {
    if (i < 0 || i >= int(expressions_.size()))
      return CJExecExpressionP();

    return expressions_[i];
  }

  Values getValues(CJavaScript *js) const {
    Values values;

    for (auto &e : expressions_) {
      assert(e);

      CJValueP value = e->exec(js);

      values.push_back(value);
    }

    return values;
  }

  CJValueP exec(CJavaScript *js) override {
    Values values = getValues(js);

    if (values.empty())
      return CJValueP();

    return values.back();
  }

  void print(std::ostream &os) const override {
    int i = 0;

    for (auto &e : expressions_) {
      if (i > 0)
        os << ", ";

      os << *e;

      ++i;
    }
  }

 private:
  Expressions expressions_;
};

typedef std::shared_ptr<CJExecExpressionList> CJExecExpressionListP;

#endif

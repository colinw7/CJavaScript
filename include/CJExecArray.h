#ifndef CJExecArray_H
#define CJExecArray_H

#include <CJToken.h>
#include <vector>
#include <iostream>

// var <identifiers> [= <value>] [, <identifiers> [= <value>], ...]
class CJExecArray : public CJToken {
 public:
  CJExecArray();

  void addValue(CJExecExpressionP expr) {
    values_.push_back(expr);
  }

  CJValueP exec(CJavaScript *js) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  typedef std::vector<CJExecExpressionP> ArrayValues;

  ArrayValues values_;
};

#endif

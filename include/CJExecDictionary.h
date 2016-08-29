#ifndef CJExecDictionary_H
#define CJExecDictionary_H

#include <CJToken.h>
#include <vector>
#include <iostream>

// var <identifiers> [= <value>] [, <identifiers> [= <value>], ...]
class CJExecDictionary : public CJToken {
 public:
  CJExecDictionary();

  void addDictionaryValue(const CJValueP &key, CJExecExpressionP expr) {
    values_.push_back(DictionaryValue(key, expr));
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  struct DictionaryValue {
    CJValueP          key;
    CJExecExpressionP expr;

    DictionaryValue(const CJValueP &key1, CJExecExpressionP expr1) :
     key(key1), expr(expr1) {
    }
  };

  typedef std::vector<DictionaryValue> DictionaryValues;

  DictionaryValues values_;
};

#endif

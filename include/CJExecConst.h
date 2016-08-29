#ifndef CJExecConst_H
#define CJExecConst_H

#include <CJToken.h>
#include <vector>
#include <iostream>

// const <identifiers> [= <value>] [, <identifiers> [= <value>], ...]
class CJExecConst : public CJToken {
 public:
  CJExecConst();

  void addVarValue(CJExecIdentifiersP identifiers, CJExecExpressionP expr) {
    varValues_.push_back(VarValue(identifiers, expr));
  }

  void addVarValue(CJExecIdentifiersP identifiers, CJExecBlockP block) {
    varValues_.push_back(VarValue(identifiers, block));
  }

  void addVarValue(CJExecIdentifiersP identifiers, CJExecArrayP array) {
    varValues_.push_back(VarValue(identifiers, array));
  }

  void addVarValue(CJExecIdentifiersP identifiers, CJExecDictionaryP dict) {
    varValues_.push_back(VarValue(identifiers, dict));
  }

  void addVarValue(CJExecIdentifiersP identifiers, CJValueP value) {
    varValues_.push_back(VarValue(identifiers, value));
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  struct VarValue {
    CJExecIdentifiersP identifiers;
    CJExecExpressionP  expr;
    CJExecBlockP       block;
    CJExecArrayP       array;
    CJExecDictionaryP  dict;
    CJValueP           value;

    VarValue(CJExecIdentifiersP identifiers1, CJExecExpressionP expr1) :
     identifiers(identifiers1), expr(expr1) {
    }

    VarValue(CJExecIdentifiersP identifiers1, CJExecBlockP block1) :
     identifiers(identifiers1), block(block1) {
    }

    VarValue(CJExecIdentifiersP identifiers1, CJExecArrayP array1) :
     identifiers(identifiers1), array(array1) {
    }

    VarValue(CJExecIdentifiersP identifiers1, CJExecDictionaryP dict1) :
     identifiers(identifiers1), dict(dict1) {
    }

    VarValue(CJExecIdentifiersP identifiers1, CJValueP value1) :
     identifiers(identifiers1), value(value1) {
    }
  };

  typedef std::vector<VarValue> VarValues;

  VarValues varValues_;
};

#endif

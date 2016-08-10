#ifndef CJExecIf_H
#define CJExecIf_H

#include <CJToken.h>
#include <vector>
#include <iostream>

// if ( <expression> ) <block> [ else <block> ]
class CJExecIf : public CJToken {
 public:
  struct ExpressionBlock {
    CJExecExpressionListP exprList;
    CJExecBlockP          block;
  };

 public:
  CJExecIf();

  void setIfBlock(CJExecExpressionListP exprList, CJExecBlockP block) {
    ifBlock_.exprList = exprList;
    ifBlock_.block    = block;
  }

  void addElseIfBlock(CJExecExpressionListP exprList, CJExecBlockP block) {
    ExpressionBlock exprBlock;

    exprBlock.exprList = exprList;
    exprBlock.block    = block;

    elseIfBlocks_.push_back(exprBlock);
  }

  void setElseBlock(CJExecBlockP block) {
    elseBlock_.block = block;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  typedef std::vector<ExpressionBlock> ExpressionBlocks;

  CJExecExpressionListP exprList_;
  ExpressionBlock       ifBlock_;
  ExpressionBlocks      elseIfBlocks_;
  ExpressionBlock       elseBlock_;
};

typedef std::shared_ptr<CJExecIf> CJExecIfP;

#endif

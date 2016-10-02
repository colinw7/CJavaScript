#ifndef CJExecSwitch_H
#define CJExecSwitch_H

#include <CJToken.h>
#include <vector>
#include <iostream>

// switch ( <expression> ) { [case <expr> : <exprList> ; break; ]* }
class CJExecSwitch : public CJToken {
 public:
  enum class Type {
    None,
    Case,
    Default
  };

  struct SwitchBlock {
    SwitchBlock() { }

    SwitchBlock(Type type1, CJExecExpressionP expr1, CJExecBlockP block1) :
     type(type1), expr(expr1), block(block1) {
    }

    Type              type { Type::None };
    CJExecExpressionP expr;
    CJExecBlockP      block;
  };

 public:
  CJExecSwitch();

  void setExprList(CJExecExpressionListP exprList) {
    exprList_ = exprList;
  }

  void addCase(CJExecExpressionP expr, CJExecBlockP block) {
    caseBlocks_.push_back(SwitchBlock(Type::Case, expr, block));
  }

  bool hasDefault() const {
    return defaultBlock_.type != Type::None;
  }

  bool addDefault(CJExecBlockP block) {
    if (hasDefault()) return false;

    defaultBlock_ = SwitchBlock(Type::Default, CJExecExpressionP(), block);

    caseBlocks_.push_back(defaultBlock_);

    return true;
  }

  CJValueP exec(CJavaScript *js) override;

  bool execBlock(CJavaScript *js, CJExecBlockP block);

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  typedef std::vector<SwitchBlock> CaseBlocks;

  CJExecExpressionListP exprList_;
  CaseBlocks            caseBlocks_;
  SwitchBlock           defaultBlock_;
};

#endif

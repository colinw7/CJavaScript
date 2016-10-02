#ifndef CJExecNew_H
#define CJExecNew_H

#include <CJToken.h>
#include <iostream>

// new <type> ( <args> )
class CJExecNew : public CJToken {
 public:
  CJExecNew();

  const std::string &typeName() const { return typeName_; }
  void setTypeName(const std::string &v) { typeName_ = v; }

  CJExecExpressionListP exprList() const { return exprList_; }
  void setExprList(CJExecExpressionListP exprList) { exprList_ = exprList; }

  CJValueP exec(CJavaScript *js) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  std::string           typeName_;
  CJExecExpressionListP exprList_;
};

#endif

#ifndef CJExecBlock_H
#define CJExecBlock_H

#include <CJToken.h>
#include <iostream>
#include <vector>

// { [<statement>]* }
class CJExecBlock : public CJToken {
 public:
  enum class Type {
    Sequential,
    Iterative,
    Function
  };

 public:
  typedef std::vector<CJTokenP> ETokens;

 public:
  CJExecBlock(Type type);

  Type type() const { return type_; }

  void addToken(CJTokenP token) {
    tokens_.push_back(token);
  }

  bool anyTokens() const {
    return ! tokens_.empty();
  }

  ETokens &etokens() { return etokens_; }

  CJValueP getRetVal() const { return retVal_; }
  void setRetVal(CJValueP v) { retVal_ = v; }

  bool isBreakFlag() const { return breakFlag_; }
  void setBreakFlag(bool b) { breakFlag_ = b; }

  bool isContinueFlag() const { return continueFlag_; }
  void setContinueFlag(bool b) { continueFlag_ = b; }

  bool isReturnFlag() const { return returnFlag_; }
  void setReturnFlag(bool b) { returnFlag_ = b; }

  bool hasError() const { return !!error_; }
  CJErrorBaseP error() const { return error_; }
  void setError(CJErrorBaseP error) { error_ = error; }
  void resetError() { error_ = CJErrorBaseP(); }

  void interp(CJavaScript *js);

  void initExec() {
    retVal_       = 0;
    breakFlag_    = false;
    continueFlag_ = false;
    returnFlag_   = false;
    error_        = CJErrorBaseP();
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  typedef std::vector<CJTokenP> Tokens;

  Type         type_ { Type::Sequential };
  Tokens       tokens_;
  ETokens      etokens_;
  CJValueP     retVal_;
  bool         breakFlag_    { false };
  bool         continueFlag_ { false };
  bool         returnFlag_   { false };
  CJErrorBaseP error_;
};

#endif

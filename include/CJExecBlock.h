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
    Switch,
    Function
  };

 public:
  typedef std::vector<CJTokenP>     ETokens;
  typedef std::vector<CJErrorBaseP> Errors;

 public:
  explicit CJExecBlock(Type type);

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

  bool isStrict() const { return strict_; }
  void setStrict(bool b) { strict_ = b; }

  const Errors &errors() { return errors_; }
  void setErrors(const Errors &errors) { errors_ = errors; }

  bool hasError() const { return !errors_.empty(); }
  CJErrorBaseP firstError() const { return (hasError() ? errors_.front() : CJErrorBaseP()); }
  void addError(CJErrorBaseP error) { errors_.push_back(error); }
  void resetErrors() { errors_.clear(); }

  void interp(CJavaScript *js);

  void initExec() {
    retVal_       = nullptr;
    breakFlag_    = false;
    continueFlag_ = false;
    returnFlag_   = false;

    errors_.clear();
  }

  CJValueP exec(CJavaScript *js) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  typedef std::vector<CJTokenP> Tokens;

  Type     type_ { Type::Sequential };
  Tokens   tokens_;
  ETokens  etokens_;
  CJValueP retVal_;
  bool     breakFlag_    { false };
  bool     continueFlag_ { false };
  bool     returnFlag_   { false };
  bool     strict_       { false };
  Errors   errors_;
};

#endif

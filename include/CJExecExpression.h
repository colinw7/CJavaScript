#ifndef CJExecExpression_H
#define CJExecExpression_H

#include <CJToken.h>
#include <vector>
#include <iostream>

// <expression>
class CJExecExpression : public CJToken {
 public:
  typedef std::vector<CJTokenP> Tokens;

 private:
  typedef std::vector<CJValueP> Values;

  class Operators {
   public:
    Operators() { }

    bool empty() const { return ops_.empty(); }

    uint size() const { return uint(ops_.size()); }

    void push_back(CJOperator *op) {
      ops_.push_back(op);
    }

    void pop_back() {
      ops_.pop_back();
    }

    CJOperator *lastOp() const {
      return (! ops_.empty() ? ops_.back() : nullptr);
    }

   private:
    typedef std::vector<CJOperator *> Ops;

    Ops ops_;
  };

 public:
  explicit CJExecExpression(Type type=CJToken::Type::Expression) :
   CJToken(type) {
  }

  void addToken(CJTokenP token) {
    tokens_.push_back(token);
  }

  bool hasTokens() const { return ! tokens_.empty(); }

  const Tokens &tokens() const { return tokens_; }

  CJTokenP token(int i) const { return tokens_[size_t(i)]; }

  CJTokenP lastToken() const {
    if (tokens_.empty())
      return CJTokenP();

    return tokens_.back();
  }

  CJToken::Type lastTokenType() const {
    if (tokens_.empty() || ! tokens_.back())
      return CJToken::Type::None;

    return tokens_.back()->type();
  }

  CJValueP exec(CJavaScript *js) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

  static bool isShortCircuit(const Operators &operators, const Values &values);

 private:
  Tokens tokens_;
};

#endif

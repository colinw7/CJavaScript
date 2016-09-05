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

    uint size() const { return ops_.size(); }

    void push_back(CJOperator *op) {
      ops_.push_back(op);
    }

    void pop_back() {
      ops_.pop_back();
    }

    CJOperator *lastOp() const {
      return (! ops_.empty() ? ops_.back() : 0);
    }

   private:
    typedef std::vector<CJOperator *> Ops;

    Ops ops_;
  };

 public:
  CJExecExpression(Type type=CJToken::Type::Expression) :
   CJToken(type) {
  }

  void addToken(CJTokenP token) {
    tokens_.push_back(token);
  }

  bool hasTokens() const { return ! tokens_.empty(); }

  const Tokens &tokens() const { return tokens_; }

  CJTokenP token(int i) const { return tokens_[i]; }

  CJToken::Type lastTokenType() const {
    if (tokens_.empty() || ! tokens_.back())
      return CJToken::Type::None;

    return tokens_.back()->type();
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    int i = 0;

    for (auto &t : tokens_) {
      if (i > 0)
        os << " ";

      os << *t;

      ++i;
    }
  }

  static bool isShortCircuit(const Operators &operators, const Values &values);

 private:
  Tokens tokens_;
};

#endif
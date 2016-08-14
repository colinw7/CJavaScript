#ifndef CJKeyword_H
#define CJKeyword_H

#include <CJToken.h>
#include <string>

class CJKeyword : public CJToken {
 public:
  enum class Type {
    None,
    Break,
    Case,
    Catch,
    Class,
    Const,
    Continue,
    Default,
    Delete,
    Do,
    Else,
    Finally,
    For,
    Function,
    If,
    In,
    New,
    Return,
    Switch,
    This,
    Throw,
    Try,
    Var,
    Void,
    While,
    With
  };

 public:
  CJKeyword(const Type &type) :
   CJToken(CJToken::Type::Keyword), type_(type) {
  }

  Type type() const { return type_; }

  std::string name() const;

  void print(std::ostream &os) const override { os << name(); }

 private:
  Type type_;
};

#endif

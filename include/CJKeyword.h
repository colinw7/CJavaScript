#ifndef CJKeyword_H
#define CJKeyword_H

#include <CJToken.h>
#include <string>

class CJKeyword : public CJToken {
 public:
  enum class Type {
    None,
    Assert,
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
    New,
    Return,
    Switch,
    This,
    Throw,
    Try,
    Var,
    While,
    With
  };

 public:
  static Type nameToType(const std::string &name);

  CJKeyword(const Type &type);

  Type type() const { return type_; }

  std::string name() const;

  void print(std::ostream &os) const override { os << name(); }

 private:
  Type type_;
};

#endif

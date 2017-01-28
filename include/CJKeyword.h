#ifndef CJKeyword_H
#define CJKeyword_H

#include <CJToken.h>
#include <string>

class CJKeyword : public CJToken {
 public:
  enum class Type {
    None,
    Assert,
    AssertEq,
    Break,
    Case,
    Catch,
    Class,
    Const,
    Continue,
    Default,
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

  explicit CJKeyword(const Type &type);

  Type type() const { return type_; }

  std::string name() const;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  Type type_;
};

#endif

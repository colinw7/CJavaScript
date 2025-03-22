#ifndef CJTokenValue_H
#define CJTokenValue_H

#include <CJObjType.h>
#include <CJValue.h>

// TokenValue Type
class CJTokenValueType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJTokenValueType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//------

// TokenValue Value
class CJTokenValue : public CJValue {
 public:
  typedef std::vector<CJTokenP> Tokens;

 public:
  CJTokenValue(CJavaScript *js, const Tokens &tokens=Tokens(), const CJValueP &value=CJValueP());

  CJTokenValue(const CJTokenValue &value);

  CJValue *dup(CJavaScript *) const override { return new CJTokenValue(*this); }

  const Tokens &tokens() const { return tokens_; }
  void setTokens(const Tokens &tokens) { tokens_ = tokens; }

  CJValueP value() const { return value_; }
  void setValue(CJValueP value);

  //---

  OptReal toReal() const override;

  OptLong toInteger() const override;

  bool toBoolean() const override;

  //---

  void addToken(CJTokenP token);

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  Tokens   tokens_;
  CJValueP value_;
};

#endif

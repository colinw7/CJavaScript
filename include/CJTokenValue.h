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

  CJValue *dup(CJavaScript *js) const { return new CJTokenValue(js, tokens_, value_); }

  const Tokens &tokens() const { return tokens_; }
  void setTokens(const Tokens &tokens) { tokens_ = tokens; }

  CJValueP value() const { return value_; }
  void setValue(CJValueP value);

  double toReal() const;

  bool toBoolean() const;

  void addToken(CJTokenP token);

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  Tokens   tokens_;
  CJValueP value_;
};

#endif

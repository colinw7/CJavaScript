#ifndef CJString_H
#define CJString_H

#include <CJObjectType.h>
#include <CJValue.h>

// String Type
class CJStringType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CJStringType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjectTypeP type_;
};

//-------

// String Value
class CJString : public CJValue {
 public:
  CJString(CJavaScript *js, const std::string &text="", char c='\"');

  CJString *dup(CJavaScript *js) const override { return new CJString(js, text_, c_); }

  bool isProtoValue() const override { return true; }

  const std::string &text() const { return text_; }
  void setText(const std::string &str) { text_ = str; }

  std::string toString() const override { return text_; }

  double toReal() const override;

  long toInteger() const override;

  bool toBoolean() const override;

  CJValueP indexValue(int i) const override;

  void setIndexValue(int i, CJValueP value) override;

  int length() const override { return text_.size(); }

  void print(std::ostream &os) const override;

 private:
  CJavaScript *js_ { 0 };
  std::string  text_;
  char         c_ { '\"' };
};

typedef std::shared_ptr<CJString> CJStringP;

#endif

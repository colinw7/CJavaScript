#ifndef CJString_H
#define CJString_H

#include <CJObj.h>

// String Type
class CJStringType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJStringType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// String Value
class CJString : public CJObj {
 public:
  static double parseFloat(const std::string &text);
  static long   parseInt  (const std::string &text);

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

  bool hasIndex() const override { return true; }

  CJValueP indexValue(int i) const override;

  void setIndexValue(int i, CJValueP value) override;

  long length() const override { return text_.size(); }

  CJValueP getProperty(const std::string &key) const override;
  void setProperty(const std::string &key, CJValueP value) override;

  void print(std::ostream &os) const override;

 private:
  CJavaScript* js_ { 0 };
  std::string  text_;
  char         c_ { '\"' };
};

typedef std::shared_ptr<CJString> CJStringP;

#endif

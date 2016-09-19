#ifndef CJString_H
#define CJString_H

#include <CJObj.h>
#include <COptVal.h>

// String Type
class CJStringType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJStringType(CJavaScript *js);

  CJValueP execType(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// String Value
class CJString : public CJObj {
 public:
  static COptReal parseFloat(const std::string &text, bool extraChars=false);
  static COptLong parseInt  (const std::string &text, bool extraChars=false);
  static COptBool parseBool (const std::string &text, bool extraChars=false);

 public:
  CJString(CJavaScript *js, const std::string &text="", char c='\"');

  CJString *dup(CJavaScript *js) const override { return new CJString(js, text_, c_); }

  bool isBasic() const { return isBasic_; }
  void setIsBasic(bool b) { isBasic_ = b; }

  bool isProtoValue() const override { return true; }

  const std::string &text() const { return text_; }
  void setText(const std::string &str) { text_ = str; }

  std::string toString() const override;

  double toReal   () const override;
  long   toInteger() const override;
  bool   toBoolean() const override;

  bool hasIndex() const override { return true; }
  bool hasIndexValue(int ind) const override;
  CJValueP indexValue(int ind) const override;
  void setIndexValue(int ind, CJValueP value) override;
  void deleteIndexValue(int ind) override;

  long length() const override { return text_.size(); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  void print(std::ostream &os) const override;

 private:
  std::string text_;
  char        c_ { '\"' };
  bool        isBasic_ { true };
};

#endif

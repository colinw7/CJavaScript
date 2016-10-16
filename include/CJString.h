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
  void init();

 private:
  static CJStringTypeP type_;
};

//-------

// String Value
class CJString : public CJObj {
 public:
  static COptReal parseFloat(const std::string &text, bool extraChars=false);
  static COptLong parseInt  (const std::string &text, const COptInt &base=COptInt(),
                             bool extraChars=false);
  static COptBool parseBool (const std::string &text, bool extraChars=false);

 public:
  CJString(CJavaScript *js, const std::string &text="", char c='\"');

  CJString *dup(CJavaScript *js) const override { return new CJString(js, text_, c_); }

  bool isPrimitive() const override { return primitive_; }
  void setIsPrimitive(bool b) { primitive_ = b; }

  bool isString() const override { return true; }

  const std::string &text() const { return text_; }
  void setText(const std::string &str) { text_ = str; }

  //---

  COptReal toReal   () const override;
  COptLong toInteger() const override;

  bool toBoolean() const override;

  //---

  bool hasIndex() const override { return true; }
  bool hasIndexValue(long ind) const override;
  CJValueP indexValue(long ind) const override;
  void setIndexValue(long ind, CJValueP value) override;
  void deleteIndexValue(long ind) override;

  COptLong length() const override;

  std::string substr(long ind) const;
  std::string substr(long ind, long n) const;

  static std::string substr(const std::string &str, long len, long ind);
  static std::string substr(const std::string &str, long len, long ind, long n);

  std::string trim(bool left, bool right) const;

  static std::string trim(const std::string &str, long len, bool left, bool right);

  bool hasProperty() const override { return ! isPrimitive(); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  std::string encodeText() const;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  std::string text_;
  char        c_ { '\"' };
  bool        primitive_ { true };
};

#endif

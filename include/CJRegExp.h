#ifndef CJRegExp_H
#define CJRegExp_H

#include <CJObj.h>
#include <CRegExp.h>

// RegExp Type
class CJRegExpType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJRegExpType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// RegExp Value
class CJRegExp : public CJObj {
 public:
  CJRegExp(CJavaScript *js, const std::string &text="", const std::string &flags="");

  CJRegExp *dup(CJavaScript *js) const override { return new CJRegExp(js, text_); }

  const std::string &text() const { return text_; }
  void setText(const std::string &str);

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  bool find(const std::string &str) const;

  void print(std::ostream &os) const override;

 private:
  void updateFlags();

 private:
  std::string text_;
  std::string flags_;
  CRegExp     regexp_;
};

typedef std::shared_ptr<CJRegExp> CJRegExpP;

#endif

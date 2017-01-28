#ifndef CJBoolean_H
#define CJBoolean_H

#include <CJObj.h>
#include <limits>
#include <sstream>

// Boolean Type
class CJBooleanType : public CJObjType {
 public:
  static CJBooleanTypeP instance(CJavaScript *js);

  explicit CJBooleanType(CJavaScript *js);

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;

  CJValueP execType(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  void init();

 private:
  static CJBooleanTypeP type_;
};

//------

// Boolean Value
class CJBoolean : public CJObj {
 public:
  explicit CJBoolean(CJavaScript *js, bool b=false);

  CJBoolean(const CJBoolean &b);

  CJBoolean *dup(CJavaScript *) const override { return new CJBoolean(*this); }

  void setBool(bool b) { b_ = b; }
  bool getBool() const { return b_; }

  //---

  std::string toString() const override;

  COptReal toReal() const override { return COptReal(b_ ? 1.0 : 0.0); }

  COptLong toInteger() const override { return COptLong(b_ ? 1 : 0); }

  bool toBoolean() const override { return b_; }

  //---

  int cmp(const CJValue *v) const override {
    bool b1 =    toBoolean();
    bool b2 = v->toBoolean();

    if (b1 < b2) return -1;
    if (b1 > b2) return  1;

    return 0;
  }

 private:
  bool b_;
};

#endif

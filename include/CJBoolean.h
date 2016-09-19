#ifndef CJBoolean_H
#define CJBoolean_H

#include <CJObj.h>
#include <limits>
#include <sstream>

// Boolean Type
class CJBooleanType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJBooleanType(CJavaScript *js);

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;

  CJValueP execType(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//------

// Boolean Value
class CJBoolean : public CJObj {
 public:
  CJBoolean(CJavaScript *js, bool b=false);

  CJBoolean *dup(CJavaScript *js) const override { return new CJBoolean(js, b_); }

  std::string toString() const override;

  double toReal() const override { return b_; }

  bool toBoolean() const override { return b_; }

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

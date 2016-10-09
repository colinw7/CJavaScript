#ifndef CJUserObject_H
#define CJUserObject_H

#include <CJObj.h>

class CJUserType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js, const std::string &name);

  CJUserType(CJavaScript *js, const std::string &name);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//------

class CJUserObject : public CJObj {
 public:
  CJUserObject(CJavaScript *js, CJObjTypeP userType, CJFunctionBaseP userFn);

  CJValue *dup(CJavaScript *) const override { return new CJUserObject(js_, userType_, userFn_); }

  CJFunctionBaseP userFn() const { return userFn_; }

  // TODO: call object toString method
  std::string toString() const override { return CJObj::toString(); }

  //---

  // TODO: call object valueOf method and convert return value to real
  COptReal toReal() const override { return CJObj::toReal(); }

  // TODO: call object valueOf method and convert return value to integer
  COptLong toInteger() const override { return CJObj::toInteger(); }

  // TODO: call object valueOf method and convert return value to bool
  bool toBoolean() const override { return CJObj::toBoolean(); }

  //---

  // TODO: call compare ? method
  int cmp(CJObjP obj) const override { return CJObj::cmp(obj); }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;
  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  CJObjTypeP      userType_;
  CJFunctionBaseP userFn_;
};

#endif

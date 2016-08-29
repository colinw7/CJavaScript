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
  CJUserObject(CJavaScript *js, CJObjTypeP userType, CJFunctionP userFn);

  CJValue *dup(CJavaScript *) const override { return new CJUserObject(js_, userType_, userFn_); }

  CJFunctionP userFn() const { return userFn_; }

  // TODO: call object toString method
  std::string toString() const override { return CJObj::toString(); }

  // TODO: call object valueOf method and convert return value to real
  double toReal() const override { return CJObj::toReal(); }

  // TODO: call object valueOf method and convert return value to integer
  long toInteger() const override { return CJObj::toInteger(); }

  // TODO: call object valueOf method and convert return value to bool
  bool toBoolean() const override { return CJObj::toBoolean(); }

  // TODO: call compare ? method
  int cmp(CJObjP obj) const override { return CJObj::cmp(obj); }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;
  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  CJObjTypeP  userType_;
  CJFunctionP userFn_;
};

typedef std::shared_ptr<CJUserObject> CJUserObjectP;

#endif

#ifndef CJUserObject_H
#define CJUserObject_H

#include <CJObj.h>

class CJUserType : public CJObjType {
 public:
  CJUserType(CJavaScript *js, const std::string &name);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

//------

class CJUserObject : public CJObj {
 public:
  CJUserObject(CJavaScript *js, CJObjTypeP userType);

  CJValue *dup(CJavaScript *) const override { return new CJUserObject(js_, type()); }

  // TODO: call object toString method
  std::string toString() const override { return CJObj::toString(); }

  // TODO: call object valueOf method and convert return value to real
  double toReal() const override { return CJObj::toReal(); }

  // TODO: call object valueOf method and convert return value to integer
  long toInteger() const override { return CJObj::toInteger(); }

  // TODO: call object valueOf method and convert return value to bool
  bool toBoolean() const override { return CJObj::toBoolean(); }

  // TODO: call compare ? methid
  int cmp(CJObjP obj) const { return CJObj::cmp(obj); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  CJavaScript *js_;
};

typedef std::shared_ptr<CJUserObject> CJUserObjectP;

#endif

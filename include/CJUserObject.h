#ifndef CJUserObject_H
#define CJUserObject_H

#include <CJObjectType.h>
#include <CJObject.h>

class CJUserType : public CJObjectType {
 public:
  CJUserType(const std::string &name) :
   CJObjectType(CJToken::Type::Object, name) {
  }

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

//------

class CJUserObject : public CJObject {
 public:
  CJUserObject(CJavaScript *js, CJObjectTypeP userType);

  CJValue *dup(CJavaScript *) const override { return new CJUserObject(js_, type()); }

  // TODO: call object toString method
  std::string toString() const override { return CJObject::toString(); }

  // TODO: call object valueOf method and convert return value to real
  double toReal() const override { return CJObject::toReal(); }

  // TODO: call object valueOf method and convert return value to integer
  long toInteger() const override { return CJObject::toInteger(); }

  // TODO: call object valueOf method and convert return value to bool
  bool toBoolean() const override { return CJObject::toBoolean(); }

  // TODO: call compare ? methid
  int cmp(CJObjectP obj) const { return CJObject::cmp(obj); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  CJavaScript *js_;
};

typedef std::shared_ptr<CJUserObject> CJUserObjectP;

#endif

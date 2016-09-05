#ifndef CJObject_H
#define CJObject_H

#include <CJObj.h>
#include <set>
#include <algorithm>

// Object Type
class CJObjectType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJObjectType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// Object Value
class CJObject : public CJObj {
 public:
  CJObject(CJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CJObject(js_); }
};

#endif

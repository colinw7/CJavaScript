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

  CJValueP execType(CJavaScript *js, const std::string &name, const Values &values) override;

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

  const std::string &typeName() const { return typeName_; }
  void setTypeName(const std::string &v) { typeName_ = v; }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 protected:
  std::string typeName_;
};

#endif

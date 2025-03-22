#ifndef CJObject_H
#define CJObject_H

#include <CJObj.h>
#include <set>
#include <algorithm>

// Object Type
class CJObjectType : public CJObjType {
 public:
  struct PropertyValues {
    CJValueP    value;
    OptBool     writable;
    OptBool     enumerable;
    OptBool     configurable;
    CJFunctionP getter;
    CJFunctionP setter;
  };

 public:
  static CJObjectTypeP instance(CJavaScript *js);

  explicit CJObjectType(CJavaScript *js);

  CJValueP execType(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  void init();

  static bool setObjNamePropertiesValues(CJavaScript *js, CJValueP obj, CJObjectP propObj);

  static bool getPropertyValues(CJavaScript *js, const CJValueP &desc,
                                PropertyValues &propValues);

  static void setIndexPropertyValues(CJavaScript *js, CJValueP obj, int ind,
                                     const PropertyValues &propValues);

  static void setNamePropertyValues(CJavaScript *js, CJValueP obj, const std::string &ind,
                                    const PropertyValues &propValues);

 private:
  static CJObjectTypeP type_;
};

//-------

// Object Value
class CJObject : public CJObj {
 public:
  explicit CJObject(CJavaScript *js);

  CJObject(const CJObject &obj);

  CJValue *dup(CJavaScript *) const override;

  bool isNative() const override { return false; }

  bool isUserDefined() const override { return true; }

  const std::string &typeName() const { return typeName_; }
  void setTypeName(const std::string &v) { typeName_ = v; }

  OptLong length() const override;
  void setLength(long n);

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  void configureProperty(CJavaScript *js, const std::string &key, CJValueP value);

  CJNameSpace::Names getPropertyNames(bool pseudo=true) const override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 protected:
  std::string typeName_;
};

#endif

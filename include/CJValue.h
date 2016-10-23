#ifndef CJValue_H
#define CJValue_H

#include <CJToken.h>
#include <COptVal.h>
#include <vector>

class CJValue : public CJToken, public std::enable_shared_from_this<CJValue> {
 public:
  typedef std::vector<std::string> KeyNames;

 public:
  template<typename T>
  static std::shared_ptr<T> cast(CJValueP value) {
    return std::static_pointer_cast<T>(value);
  }

 public:
  CJValue(CJObjTypeP valueType);

  CJValue(const CJValue &v);

  virtual ~CJValue() { }

  virtual CJValue *dup(CJavaScript *) const = 0;

  virtual CJObjTypeP valueType() const { return valueType_; }

  virtual bool isObject    () const { return false; }
  virtual bool isDictionary() const { return false; }
  virtual bool isArray     () const { return false; }
  virtual bool isFunction  () const { return false; }
  virtual bool isError     () const { return false; }
  virtual bool isNumber    () const { return false; }
  virtual bool isString    () const { return false; }
  virtual bool isPrimitive () const { return false; }

  //---

  // convert to string
  virtual std::string toString() const override = 0;

  virtual COptReal toReal() const = 0;

  virtual COptLong toInteger() const;

  virtual bool toBoolean() const = 0;

  //---

  bool isValue() const override { return true; }

  virtual bool hasIndex() const { return false; }
  virtual bool hasIndexValue(long) const { return false; }
  virtual CJValueP indexValue(long) const { assert(false); return CJValueP(); }
  virtual void setIndexValue(long, CJValueP) { assert(false); }
  virtual void deleteIndexValue(long) { assert(false); }

  virtual bool isWritableIndex(long) const { return true; }
  virtual void setWritableIndex(long, bool) { assert(false); }
  virtual bool isEnumerableIndex(long) const { return true; }
  virtual void setEnumerableIndex(long, bool) { assert(false); }
  virtual bool isConfigurableIndex(long) const { return true; }
  virtual void setConfigurableIndex(long, bool) { assert(false); }

  virtual bool hasProperty() const { return false; }
  virtual bool hasPropertyValue(const std::string &, bool) const { return false; }
  virtual CJValueP propertyValue(const std::string &) const { assert(false); return CJValueP(); }
  virtual void setPropertyValue(const std::string &, CJValueP) { assert(false); }
  virtual void configPropertyValue(const std::string &, CJValueP) { assert(false); }
  virtual void deletePropertyValue(const std::string &) { assert(false); }

  virtual KeyNames propertyNames() const { return KeyNames(); }

  virtual COptLong length() const { return COptLong(); }

  virtual int cmp(const CJValue *v) const {
    std::string s1 =    toString();
    std::string s2 = v->toString();

    if (s1 < s2) return -1;
    if (s1 > s2) return  1;

    return 0;
  }

 protected:
  CJObjTypeP valueType_;
};

#endif

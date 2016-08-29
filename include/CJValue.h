#ifndef CJValue_H
#define CJValue_H

#include <CJToken.h>

class CJValue : public CJToken, public std::enable_shared_from_this<CJValue> {
 public:
  CJValue(CJObjTypeP valueType);

  virtual ~CJValue() { }

  virtual CJValue *dup(CJavaScript *) const = 0;

  virtual CJObjTypeP valueType() const { return valueType_; }

  virtual bool isObject() const { return false; }

  virtual std::string toString() const override = 0;

  virtual double toReal() const = 0;

  virtual long toInteger() const { return int(toReal()); }

  virtual bool toBoolean() const = 0;

  bool isValue() const override { return true; }

  virtual bool hasIndex() const { return false; }

  virtual CJValueP indexValue(int) const { assert(false); return CJValueP(); }

  virtual void setIndexValue(int, CJValueP) { assert(false); }

  virtual long length() const { return 0; }

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

#ifndef CJObjValue_H
#define CJObjValue_H

#include <CJObjType.h>
#include <CJValue.h>
#include <CJNameSpace.h>
#include <sstream>

// ObjValue Type
class CJObjValueType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJObjValueType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//------

// ObjValue Value
class CJObjValue : public CJValue, public CJNameSpace {
 public:
  CJObjValue(CJavaScript *js, CJObjP obj, const std::string &name);

  CJValue *dup(CJavaScript *js) const override { return new CJObjValue(js, obj_, name_); }

  CJavaScript *js() const { return js_; }

  CJObjP obj() const { return obj_; }

  const std::string &name() const { return name_; }

  CJValueP getValue() const;
  void setValue(CJValueP value);

  //---

  bool toBoolean() const override;

  COptReal toReal() const override;

  COptLong toInteger() const override;

  //---

  std::string toString() const override;

  void print(std::ostream &os) const override;

  friend std::ostream &operator<<(std::ostream &os, const CJObjValue &gs) {
    gs.print(os);

    return os;
  }

 protected:
  CJavaScript* js_ { 0 };
  CJObjP       obj_;
  std::string  name_;
};

#endif

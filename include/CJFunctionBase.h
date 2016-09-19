#ifndef CJFunctionBase_H
#define CJFunctionBase_H

#include <CJObj.h>
#include <CJDictionary.h>
#include <vector>

// Function Type
class CJFunctionBaseType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js, const std::string &name);

  CJFunctionBaseType(CJavaScript *js, const std::string &name);

  const std::string &name() const { return name_; }

  CJValueP execType(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  typedef std::map<std::string,CJObjTypeP> NameTypeMap;

  static NameTypeMap nameTypeMap_;

  std::string name_;
};

//------

// Function Value
class CJFunctionBase : public CJObj {
 public:
  enum class Type {
    Normal,
    Real,
    Real2,
    Min,
    Max,
    Random,
    Type,
    ObjType,
    Call,
    System,
    ObjectType,
    Object,
    User,
    Global,
    JSON
  };

  typedef std::vector<CJValueP> Values;

 public:
  CJFunctionBase(CJavaScript *js, const std::string &name="", Type type=Type::Normal);

  CJFunctionBase(CJavaScript *js, const CJObjTypeP &objType,
                 const std::string &name="", Type type=Type::Normal);

  CJValue *dup(CJavaScript *) const override { assert(false); return 0; }

  bool isFunction() const override { return true; }

  bool isProtoValue() const override { return true; }

  const std::string &name() const { return name_; }

  std::string toString() const override { return name_; }

  virtual bool hasObjectValue() const { return false; }

  const Type &type() const { return type_; }

  const CJDictionaryP &prototype() const { return prototype_; }

  virtual CJValueP exec(CJavaScript *js, const Values &values) = 0;

  void print(std::ostream &os) const override {
    os << name_ << "()";
  }

 protected:
  std::string   name_;
  Type          type_;
  CJDictionaryP prototype_;
};

#endif

#ifndef CJPropertyData_H
#define CJPropertyData_H

#include <CJTypes.h>

class CJavaScript;

// TODO: turn into class with accessor and lvalue auto creation
class CJPropertyData {
 public:
  explicit CJPropertyData(CJavaScript *js) : js_(js) { }

  bool isModifiable() const { return modifiable_; }
  void setModifiable(bool b) { modifiable_ = b; }

  bool isCreate() const { return create_; }
  void setCreate(bool b) { create_ = b; }

  bool isCreateLast() const { return createLast_; }
  void setCreateLast(bool b) { createLast_ = b; }

  CJNameSpaceP scope() const { return scope_; }
  void setScope(const CJNameSpaceP &scope) { scope_ = scope; }

  CJObjP obj() const { return obj_; }
  void setObj(const CJObjP &obj) { obj_ = obj; }

  CJValueP objValue() const { return objValue_; }
  void setObjValue(const CJValueP &objValue) { objValue_ = objValue; }

  const std::string &name() const { return name_; }
  void setName(const std::string &name) { name_ = name; }

  CJValueP value() const;
  void setValue(const CJValueP &value) { value_ = value; }

  CJLValueP lvalue() const;

 private:
  CJavaScript*      js_ { 0 };
  bool              modifiable_ { false };
  bool              create_ { false };
  bool              createLast_ { false };
  CJNameSpaceP      scope_;
  CJObjP            obj_;
  CJValueP          objValue_;
  std::string       name_;
  mutable CJValueP  value_;
  mutable CJLValueP lvalue_;
};

#endif

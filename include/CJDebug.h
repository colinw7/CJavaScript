#ifndef CJDebug_H
#define CJDebug_H

#include <CJObjectType.h>
#include <CJObject.h>

// Debug Type
class CJDebugType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CJDebugType();

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjectTypeP type_;
};

// Debug Object
class CJDebugObject : public CJObject {
 public:
  CJDebugObject(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJDebugObject(js); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "debug"; }

 private:
  CJavaScript *js_;
};

#endif

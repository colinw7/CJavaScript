#ifndef CJDebug_H
#define CJDebug_H

#include <CJObj.h>

// Debug Type
class CJDebugType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJDebugType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

// Debug Object
class CJDebugObject : public CJObj {
 public:
  CJDebugObject(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJDebugObject(js); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "debug"; }
};

#endif

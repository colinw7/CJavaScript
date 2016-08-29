#ifndef CJConsole_H
#define CJConsole_H

#include <CJObj.h>

// Console Type
class CJConsoleType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJConsoleType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//------

// Console Object
class CJConsoleObject : public CJObj {
 public:
  CJConsoleObject(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJConsoleObject(js); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;
};

#endif

#ifndef CJConsole_H
#define CJConsole_H

#include <CJObjectType.h>
#include <CJObject.h>

// Console Type
class CJConsoleType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CJConsoleType();

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjectTypeP type_;
};

// Console Object
class CJConsoleObject : public CJObject {
 public:
  CJConsoleObject(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJConsoleObject(js); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  CJavaScript *js_;
};

#endif

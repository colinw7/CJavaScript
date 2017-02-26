#ifndef CQJLocation_H
#define CQJLocation_H

#include <CQJObject.h>

class CQJWindow;
class CQJLocation;

class CQJLocationType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJLocationType(CJavaScript *js);

  bool hasConstructor() const { return true; }

  CJValueP construct(CJavaScript *js, const Values &values) override;

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

typedef std::shared_ptr<CQJLocation> CQJLocationP;

class CQJLocation : public CQJObject {
 public:
  CQJLocation(CJavaScript *js, CQJWindow *window);

  CJValue *dup(CJavaScript *js) const override { return new CQJLocation(js, window_); }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;
  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "location"; }

 private:
  CJValueP   value_;
  CQJWindow* window_ { nullptr };
};

#endif

#ifndef CQJImage_H
#define CQJImage_H

#include <CQJObject.h>
#include <QImage>

class CQJavaScript;

class CQJImageType : public CJObjectType {
 public:
  CQJImageType(CQJavaScript *js);

  bool hasConstructor() const { return true; }

  CJValueP construct(CJavaScript *js, const Values &values) override;

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  CQJavaScript *js_ { 0 };
};

class CQJImage : public CQJObject {
 public:
  enum class Repeat {
    None,
    X,
    Y
  };

 public:
  CQJImage(CQJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CQJImage(js_); }

  QImage qimage() const { return qimage_; }

  const Repeat &repeat() const { return repeat_; }
  void setRepeat(const Repeat &v) { repeat_ = v; }

  void setProperty(const std::string &name, CJValueP value);

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "image"; }

 private:
  QImage qimage_;
  Repeat repeat_;
};

#endif

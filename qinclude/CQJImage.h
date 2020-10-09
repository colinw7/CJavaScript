#ifndef CQJImage_H
#define CQJImage_H

#include <CQJObject.h>
#include <CJObjTypeFunction.h>
#include <QImage>

class CQJImageType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJImageType(CJavaScript *js);

  bool hasConstructor() const { return true; }

  CJValueP construct(CJavaScript *js, const Values &values) override;

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJImageFunction : public CJObjTypeFunction {
 public:
  explicit CQJImageFunction(CJavaScript *js);

  CQJImageFunction(const CQJImageFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CQJImageFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  CJValueP execNew(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

//------

class CQJImage : public CQJObject {
 public:
  enum class Repeat {
    None,
    X,
    Y
  };

 public:
  explicit CQJImage(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CQJImage(js); }

  QImage qimage() const { return qimage_; }

  const Repeat &repeat() const { return repeat_; }
  void setRepeat(const Repeat &v) { repeat_ = v; }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;
  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "image"; }

 private:
  QImage          qimage_;
  Repeat          repeat_;
  CJFunctionBaseP onLoad_;
};

typedef std::shared_ptr<CQJImage> CQJImageP;

#endif

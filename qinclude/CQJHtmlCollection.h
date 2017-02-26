#ifndef CQJHtmlCollection_H
#define CQJHtmlCollection_H

#include <CJObj.h>
#include <CQJObject.h>

class CQJHtmlCollectionType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJHtmlCollectionType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJHtmlObj;

class CQJHtmlCollection : public CQJObject {
  Q_OBJECT

 public:
  explicit CQJHtmlCollection(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CQJHtmlCollection(js); }

  void addObject(CQJHtmlObj *obj);

  bool hasIndex() const override { return true; }

  CJValueP indexValue(long i) const override;
  void setIndexValue(long i, CJValueP value) override;

  CJValueP getProperty(CJavaScript *js, const std::string &name) const;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "htmlCollection"; }

 private:
  typedef std::vector<CQJHtmlObj *> Objects;

  Objects objs_;
};

#endif

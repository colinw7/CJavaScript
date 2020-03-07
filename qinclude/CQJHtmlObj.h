#ifndef CQJHtmlObj_H
#define CQJHtmlObj_H

#include <CQJObject.h>

class CQJHtmlObjType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJHtmlObjType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJHtmlObj;

typedef std::shared_ptr<CQJHtmlObj> CQJHtmlObjP;

//------

class CQJHtmlObjIFace;

class CQJHtmlObj : public CQJObject {
  Q_OBJECT

 public:
  typedef std::vector<CQJHtmlObj *> Children;

 public:
  CQJHtmlObj(CJavaScript *js, CQJHtmlObjIFace *iface);
  CQJHtmlObj(CJavaScript *js, CQJHtmlObjIFace *iface, const CJObjTypeP &type);

  virtual ~CQJHtmlObj() { }

  CJValue *dup(CJavaScript *js) const override { return new CQJHtmlObj(js, iface_); }

  void init() override;

  void setIFace(CQJHtmlObjIFace *iface) { iface_ = iface; }

  //--

  std::string getName () const;
  std::string getClass() const;
  std::string getType () const;
  std::string getId   () const;

  CQJHtmlObj *parent() const;

  Children children() const;

  //--

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;

  void setProperty(CJavaScript *js, const std::string &name, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  bool callEventListener(const std::string &name, const std::string &prop,
                         const EventArgs &args=EventArgs(),
                         const NameValues &nameValues=NameValues()) override;

  void print(std::ostream &os) const override { os << "html"; }

 protected:
  CQJHtmlObjIFace *iface_ { nullptr };
};

#endif

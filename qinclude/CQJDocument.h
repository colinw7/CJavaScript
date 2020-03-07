#ifndef CQJDocument_H
#define CQJDocument_H

#include <CQJObject.h>
#include <CUrl.h>

class CQJDocumentType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJDocumentType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJDocument;

typedef std::shared_ptr<CQJDocument> CQJDocumentP;

//------

#include <CQJWindow.h>

class CQJDocumentIFace;

class CQJDocument : public CQJObject {
  Q_OBJECT

 public:
  explicit CQJDocument(CJavaScript *js, CQJWindowP window);

  CJValue *dup(CJavaScript *js) const override { return new CQJDocument(js, window_); }

  const CUrl &url() const { return url_; }
  void setUrl(const CUrl &u) { url_ = u; }

  void setIFace(CQJDocumentIFace *iface) { iface_ = iface; }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "document"; }

 private:
  CQJWindowP        window_;               // parent window
  CQJDocumentIFace* iface_    { nullptr }; // javascript interface
  CUrl              url_;                  // current url
};

#endif

#ifndef CQJavaScript_H
#define CQJavaScript_H

#include <CJavaScript.h>
#include <CQJWindow.h>
#include <CQJDocument.h>
#include <vector>

class CJavaScript;
class CQJWindowIFace;
class CQJHtmlObj;
class CQJObject;

#define CQJavaScriptInst CQJavaScript::instance()

class CQJavaScript {
 public:
  typedef std::vector<CJValueP> EventArgs;
  typedef std::set<CQJObject *> Objects;

 public:
  static CQJavaScript *instance();

  void init();

  CJavaScript *js() const { return js_; }

  void setWindowIFace(CQJWindowIFace *windowIFace);

  CJValueP jsDocumentValue() const { return jsDocument_; }
  CJValueP jsWindowValue  () const { return jsWindow_; }

  CQJWindowP   jsWindow  () const;
  CQJDocumentP jsDocument() const;

  void addObject   (CQJObject *obj);
  void removeObject(CQJObject *obj);

  const Objects &objects() const { return objects_; }

  void addHtmlObject(CQJHtmlObj *obj);

  CQJHtmlObj *getObjectById(const std::string &id) const;

  void getObjectsByClassName(const std::string &name, std::vector<CQJHtmlObj *> &objs) const;
  void getObjectsByTypeName (const std::string &name, std::vector<CQJHtmlObj *> &objs) const;

  void getObjectsBySelector(const std::string &id, std::vector<CQJHtmlObj *> &objs) const;

  void runScript(const std::string &text);
  void runScriptFile(const std::string &filename);

  void onLoad();

  void callEventListeners(const std::string &name, const std::string &prop,
                          const EventArgs &args=EventArgs());

 private:
  CQJavaScript();

  CQJavaScript(const CQJavaScript &);
  CQJavaScript &operator=(const CQJavaScript &);

 private:
  typedef std::map<std::string,CQJHtmlObj *> IdObjMap;

  CJavaScript    *js_          { nullptr };
  CQJWindowIFace *windowIFace_ { nullptr };
  CJValueP        jsWindow_;
  CJValueP        jsDocument_;
  Objects         objects_;    // all CQJObject
  IdObjMap        idObjMap_;   // CQJHtmlObj by id
};

#endif

#ifndef CJRegExp_H
#define CJRegExp_H

#include <CJObj.h>
#include <CRegExp.h>

// RegExp Type
class CJRegExpType : public CJObjType {
 public:
  static CJRegExpTypeP instance(CJavaScript *js);

  CJRegExpType(CJavaScript *js);

  CJValueP execType(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  void init();

 private:
  static CJRegExpTypeP type_;
};

//-------

// RegExp Value
class CJRegExp : public CJObj {
 public:
  struct MatchData {
    typedef std::pair<int,int>       Range;
    typedef std::vector<Range>       Ranges;
    typedef std::vector<std::string> Matches;

    Range   range;
    Ranges  subRanges;
    Matches subMatches;
  };

 public:
  CJRegExp(CJavaScript *js, const std::string &text="", const std::string &flags="");

  CJRegExp *dup(CJavaScript *js) const override { return new CJRegExp(js, text_); }

  const std::string &text() const { return text_; }
  void setText(const std::string &str);

  void setIgnoreCase(bool b) { regexp_.setCaseSensitive(! b); }
  void setGlobalMatch(bool) { }
  void setMultiLine(bool) { }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  bool find(const std::string &str, MatchData &data) const;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  void updateFlags();

 private:
  std::string text_;
  std::string flags_;
  CRegExp     regexp_;
};

#endif

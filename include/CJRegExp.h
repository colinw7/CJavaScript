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
  CJRegExp(CJavaScript *js, const std::string &text="");

  CJRegExp(const CJRegExp &regexp);

  CJRegExp *dup(CJavaScript *) const override { return new CJRegExp(*this); }

  const std::string &text() const { return text_; }
  void setText(const std::string &str);

  bool isIgnoreCase() const { return ! regexp_.isCaseSensitive(); }
  void setIgnoreCase(bool b) { regexp_.setCaseSensitive(! b); }

  bool isGlobalMatch() const { return global_; }
  void setGlobalMatch(bool b) { global_ = b; }

  bool isMultiLine() const { return multiLine_; }
  void setMultiLine(bool b) { multiLine_ = b; }

  long lastIndex() const { return lastIndex_; }
  void setLastIndex(long i) { lastIndex_ = i; }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  bool find(const std::string &str, MatchData &data) const;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  std::string text_;
  bool        global_ { false };
  bool        multiLine_ { false };
  long        lastIndex_ { 0 };
  CRegExp     regexp_;
};

#endif

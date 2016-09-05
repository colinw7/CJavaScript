#ifndef CJExecData_H
#define CJExecData_H

class CJExecData {
 public:
  typedef std::vector<CJTokenP> Tokens;
  typedef std::vector<CJTokenP> ETokens;

 public:
  CJExecData() { }

  bool isBlock() const { return block_; }
  void setBlock(bool b) { block_ = b; }

  bool eof() const {
    return (pos_ >= len_);
  }

  void initExec(Tokens &tokens) {
    len_    = tokens.size();
    tokens_ = tokens;
  }

  void addEToken(CJTokenP etoken) {
    etokens_.push_back(etoken);
  }

  int pos() const { return pos_; }
  void setPos(int i) { pos_ = i; }

  const ETokens &etokens() const { return etokens_; }

  CJValueP unstackEValue(CJavaScript *js) {
    CJValueP value;

    for (auto &t : etokens_) {
      value = t->exec(js);
    }

    etokens_.clear();

    return value;
  }

  CJTokenP token() const {
    if (eof()) return 0;

    return tokens_[pos_];
  }

  void next() {
    ++pos_;
  }

 private:
  int     pos_ { 0 };
  int     len_ { 0 };
  Tokens  tokens_;
  ETokens etokens_;
  bool    block_ { false };
};

#endif

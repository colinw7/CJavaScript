#ifndef CQJHtmlObjCSSTagData_H
#define CQJHtmlObjCSSTagData_H

#include <CCSS.h>
#include <CQJHtmlObj.h>

class CQJHtmlObjCSSTagData : public CCSSTagData {
 public:
  typedef std::vector<CQJHtmlObj *> Children;

 public:
  explicit CQJHtmlObjCSSTagData(CQJHtmlObj *obj) :
   obj_(obj) {
  }

  CQJHtmlObj *obj() const { return obj_; }

  bool isElement(const std::string &name) const override {
    return (name == obj_->getType());
  }

  bool isClass(const std::string &name) const override {
#if 0
    for (const auto &c : obj_->getClasses()) {
      if (name == c)
        return true;
    }

    return false;
#else
    return (name == obj_->getClass());
#endif
  }

  bool isId(const std::string &name) const override {
    return (name == obj_->getId());
  }

  bool hasAttribute(const std::string & /*name*/, CCSSAttributeOp /*op*/,
                    const std::string & /*value*/) const override {
#if 0
    CHtmlTag *tag = obj_->tag();
    if (! tag) return false;

    if      (op == CCSSAttributeOp::NONE)
      return tag->hasOptionName(name);
    else if (op == CCSSAttributeOp::EQUAL)
      return tag->hasOptionNameValue(name, value);
    else if (op == CCSSAttributeOp::PARTIAL)
      return tag->hasOptionNameSubValue(name, value);
    else if (op == CCSSAttributeOp::STARTS_WITH)
      return tag->hasOptionNameStart(name, value);
    else
      return false;
#else
    return false;
#endif
  }

  bool isNthChild(int n) const override {
    if (! obj_->parent())
      return false;

    CQJHtmlObj::Children children = obj_->parent()->children();

    int i = 1;

    for (const auto &child : children) {
      if (child == obj_)
        return (i == n);

      ++i;
    }

    return false;
  }

  bool isInputValue(const std::string & /*name*/) const override {
#if 0
    if (obj_->type() != CHtmlTagId::INPUT)
      return false;

    CQJFormInputIFace *input = dynamic_cast<CQJFormInputIFace *>(obj_);
    if (! input) return false;

    std::string value;

    return input->getNameValue(name, value);
#else
    return false;
#endif
  }

  CCSSTagDataP getParent() const override {
    if (! obj_->parent())
      return CCSSTagDataP();

    return CCSSTagDataP(new CQJHtmlObjCSSTagData(obj_->parent()));
  }

  void getChildren(TagDataArray &children) const override {
    CQJHtmlObj::Children children1 = obj_->children();

    for (const auto &child : children1) {
      CCSSTagDataP dataP(new CQJHtmlObjCSSTagData(child));

      children.push_back(dataP);
    }
  }

  CCSSTagDataP getPrevSibling() const override {
    if (! obj_->parent())
      return CCSSTagDataP();

    CQJHtmlObj::Children children = obj_->parent()->children();

    CQJHtmlObj *lastChild = nullptr;

    for (const auto &child : children) {
      if (child == obj_) {
        if (lastChild)
          return CCSSTagDataP(new CQJHtmlObjCSSTagData(lastChild));
      }

      lastChild = child;
    }

    return CCSSTagDataP();
  }

  CCSSTagDataP getNextSibling() const override {
    if (! obj_->parent())
      return CCSSTagDataP();

    bool nextChild = false;

    CQJHtmlObj::Children children = obj_->parent()->children();

    for (const auto &child : children) {
      if (nextChild)
        return CCSSTagDataP(new CQJHtmlObjCSSTagData(child));

      if (child == obj_)
        nextChild = true; // return next child
    }

    return CCSSTagDataP();
  }

 private:
  CQJHtmlObj *obj_ { nullptr };
};

#endif

#ifndef CJObjectType_H
#define CJObjectType_H

#include <CJNameSpace.h>
#include <CJToken.h>
#include <set>

class CJObjectType : public CJNameSpace, public std::enable_shared_from_this<CJObjectType> {
 public:
  typedef std::set<std::string> Functions;
  typedef std::vector<CJValueP> Values;

 public:
  CJObjectType(const CJToken::Type type, const std::string &name) :
   type_(type), name_(name) {
  }

  const CJToken::Type &type() const { return type_; }

  const std::string &name() const { return name_; }

  void addFunction(CJavaScript *js, const std::string &name);

  virtual bool hasConstructor() const { return false; }

  virtual CJValueP construct(CJavaScript *, const Values &) { return CJValueP(); }

  virtual CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) = 0;

  friend std::ostream &operator<<(std::ostream &os, const CJObjectType &rhs) {
    os << rhs.name_;

    return os;
  }

 protected:
  CJToken::Type type_;
  std::string   name_;
};

typedef std::shared_ptr<CJObjectType> CJObjectTypeP;

#endif

#ifndef CJError_H
#define CJError_H

#include <CJObj.h>

//-------

// Error Type Base
class CJErrorTypeBase : public CJObjType {
 public:
  CJErrorTypeBase(CJavaScript *js, CJToken::Type type, const std::string &name);

  CJValueP getProperty(CJavaScript *js, const std::string &key) const;

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

  CJObjTypeP parentType() const { return parentType_; }

 protected:
  CJObjTypeP parentType_;
};

// Error Base
class CJErrorBase : public CJObj {
 public:
  CJErrorBase(CJavaScript *js, CJObjTypeP type, const std::string &name);

  const std::string &name() const { return name_; }
  void setName(const std::string &s) { name_ = s; }

  const std::string &message() const { return message_; }
  void setMessage(const std::string &s) { message_ = s; }

  CJExceptionType type() const { return type_; }
  void setType(CJExceptionType type) { type_ = type; }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  void print(std::ostream &os) const override;

 protected:
  std::string     name_;
  std::string     message_;
  CJExceptionType type_ { CJExceptionType::NONE };
};

//-------

// Error Type
class CJErrorType : public CJErrorTypeBase {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// Error Value
class CJError : public CJErrorBase {
 public:
  CJError(CJavaScript *js);

  CJError *dup(CJavaScript *js) const override { return new CJError(js); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

//------

// TypeError Type
class CJTypeErrorType : public CJErrorTypeBase {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJTypeErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// TypeError Value
class CJTypeError : public CJErrorBase {
 public:
  CJTypeError(CJavaScript *js);

  CJTypeError *dup(CJavaScript *js) const override { return new CJTypeError(js); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

//------

// ReferenceError Type
class CJReferenceErrorType : public CJErrorTypeBase {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJReferenceErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// ReferenceError Value
class CJReferenceError : public CJErrorBase {
 public:
  CJReferenceError(CJavaScript *js);

  CJReferenceError *dup(CJavaScript *js) const override { return new CJReferenceError(js); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

//------

// EvalError Type
class CJEvalErrorType : public CJErrorTypeBase {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJEvalErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// EvalError Value
class CJEvalError : public CJErrorBase {
 public:
  CJEvalError(CJavaScript *js);

  CJEvalError *dup(CJavaScript *js) const override { return new CJEvalError(js); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

//------

// RangeError Type
class CJRangeErrorType : public CJErrorTypeBase {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJRangeErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// RangeError Value
class CJRangeError : public CJErrorBase {
 public:
  CJRangeError(CJavaScript *js);

  CJRangeError *dup(CJavaScript *js) const override { return new CJRangeError(js); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

//------

// SyntaxError Type
class CJSyntaxErrorType : public CJErrorTypeBase {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJSyntaxErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// SyntaxError Value
class CJSyntaxError : public CJErrorBase {
 public:
  CJSyntaxError(CJavaScript *js);

  CJSyntaxError *dup(CJavaScript *js) const override { return new CJSyntaxError(js); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

//------

// URIError Type
class CJURIErrorType : public CJErrorTypeBase {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJURIErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// URIError Value
class CJURIError : public CJErrorBase {
 public:
  CJURIError(CJavaScript *js);

  CJURIError *dup(CJavaScript *js) const override { return new CJURIError(js); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

#endif

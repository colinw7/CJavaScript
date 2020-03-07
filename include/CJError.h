#ifndef CJError_H
#define CJError_H

#include <CJObj.h>
#include <CJTypes.h>

//-------

// Error Type Base
class CJErrorTypeBase : public CJObjType {
 public:
  CJErrorTypeBase(CJavaScript *js, CJToken::Type type, const std::string &name);

  void init(CJObjTypeP type);

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

  CJObjTypeP parentType() const override { return parentType_; }

 protected:
  static CJObjectTypeFunctionP toStringFn_;

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

  CJExceptionType exceptionType() const { return exceptionType_; }
  void setExceptionType(CJExceptionType type) { exceptionType_ = type; }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  bool isError() const override { return true; }

  std::string toString() const override;

  void print(std::ostream &os) const override;

 protected:
  std::string     name_;
  std::string     message_;
  CJExceptionType exceptionType_ { CJExceptionType::NONE };
};

//-------

// Error Type
class CJErrorType : public CJErrorTypeBase {
 public:
  static CJErrorTypeP instance(CJavaScript *js);

  CJErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJErrorTypeP type_;
};

//-------

// Error Value
class CJError : public CJErrorBase {
 public:
  CJError(CJavaScript *js);

  CJError(const CJError &error);

  CJError *dup(CJavaScript *) const override { return new CJError(*this); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  CJValueP value() const { return value_; }
  void setValue(CJValueP value) { value_ = value; }

 private:
  CJValueP value_;
};

//------

// TypeError Type
class CJTypeErrorType : public CJErrorTypeBase {
 public:
  static CJTypeErrorTypeP instance(CJavaScript *js);

  CJTypeErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJTypeErrorTypeP type_;
};

//-------

// TypeError Value
class CJTypeError : public CJErrorBase {
 public:
  CJTypeError(CJavaScript *js);

  CJTypeError(const CJTypeError &error);

  CJTypeError *dup(CJavaScript *) const override { return new CJTypeError(*this); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

//------

// ReferenceError Type
class CJReferenceErrorType : public CJErrorTypeBase {
 public:
  static CJReferenceErrorTypeP instance(CJavaScript *js);

  CJReferenceErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJReferenceErrorTypeP type_;
};

//-------

// ReferenceError Value
class CJReferenceError : public CJErrorBase {
 public:
  CJReferenceError(CJavaScript *js);

  CJReferenceError(const CJReferenceError &error);

  CJReferenceError *dup(CJavaScript *) const override { return new CJReferenceError(*this); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

//------

// EvalError Type
class CJEvalErrorType : public CJErrorTypeBase {
 public:
  static CJEvalErrorTypeP instance(CJavaScript *js);

  CJEvalErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJEvalErrorTypeP type_;
};

//-------

// EvalError Value
class CJEvalError : public CJErrorBase {
 public:
  CJEvalError(CJavaScript *js);

  CJEvalError(const CJEvalError &error);

  CJEvalError *dup(CJavaScript *) const override { return new CJEvalError(*this); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

//------

// RangeError Type
class CJRangeErrorType : public CJErrorTypeBase {
 public:
  static CJRangeErrorTypeP instance(CJavaScript *js);

  CJRangeErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJRangeErrorTypeP type_;
};

//-------

// RangeError Value
class CJRangeError : public CJErrorBase {
 public:
  CJRangeError(CJavaScript *js);

  CJRangeError(const CJRangeError &error);

  CJRangeError *dup(CJavaScript *) const override { return new CJRangeError(*this); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

//------

// SyntaxError Type
class CJSyntaxErrorType : public CJErrorTypeBase {
 public:
  static CJSyntaxErrorTypeP instance(CJavaScript *js);

  CJSyntaxErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJSyntaxErrorTypeP type_;
};

//-------

// SyntaxError Value
class CJSyntaxError : public CJErrorBase {
 public:
  CJSyntaxError(CJavaScript *js);

  CJSyntaxError(const CJSyntaxError &error);

  CJSyntaxError *dup(CJavaScript *) const override { return new CJSyntaxError(*this); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

//------

// URIError Type
class CJURIErrorType : public CJErrorTypeBase {
 public:
  static CJURIErrorTypeP instance(CJavaScript *js);

  CJURIErrorType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJURIErrorTypeP type_;
};

//-------

// URIError Value
class CJURIError : public CJErrorBase {
 public:
  CJURIError(CJavaScript *js);

  CJURIError(const CJURIError &error);

  CJURIError *dup(CJavaScript *) const override { return new CJURIError(*this); }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;
};

#endif

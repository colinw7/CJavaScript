#include <CJavaScript.h>
#include <CFile.h>
#include <CStrUtil.h>
#include <CStrParse.h>
#include <COSNaN.h>
#include <iostream>
#include <cmath>

namespace {
  double getNaN() {
    double r;

    COSNaN::set_nan(&r);

    return r;
  }

  int hexCharValue(char c) {
    if (isdigit(c)) return (c - '0');

    return (tolower(c) - 'a' + 10);
  }

  double realModulus(double real1, double real2, int *error_code=0) {
    if (error_code) *error_code = 0;

    if (COSNaN::is_nan(real1) || COSNaN::is_nan(real2)) {
      //if (error_code) *error_code = int(CExprErrorType::NAN_OPERATION);
      return getNaN();
    }

    if (real2 == 0.0) {
      //if (error_code) *error_code = int(CExprErrorType::DIVIDE_BY_ZERO);
      return getNaN();
    }

    int factor = int(real1/real2);

    double result = real1 - (real2*factor);

    return result;
  }
}

//---

CJavaScript::
CJavaScript()
{
  // Types
  undefinedType_ = CJObjectTypeP(new CJUndefinedType);
  nullType_      = CJObjectTypeP(new CJNullType);
  trueType_      = CJObjectTypeP(new CJTrueType);
  falseType_     = CJObjectTypeP(new CJFalseType);
  numberType_    = CJObjectTypeP(new CJNumberType);
  stringType_    = CJObjectTypeP(new CJStringType);
  arrayType_     = CJObjectTypeP(new CJArrayType);
  dictType_      = CJObjectTypeP(new CJDictionaryType);
  funcType_      = CJObjectTypeP(new CJFunctionType);

  scope_ = CJScopeP(new CJScope(this, ""));

  // String
  stringType_->addFunction("charAt");
  stringType_->addFunction("concat");
  stringType_->addFunction("indexOf");
  stringType_->addFunction("lastIndexOf");
  stringType_->addFunction("length");
  stringType_->addFunction("replace");
  stringType_->addFunction("slice");
  stringType_->addFunction("split");
  stringType_->addFunction("substr");
  stringType_->addFunction("substring");
  stringType_->addFunction("toLowerCase");
  stringType_->addFunction("toUpperCase");

  addTypeObject(CJToken::Type::String, stringType_);

  // Array
  arrayType_ = CJObjectTypeP(new CJArrayType);

  arrayType_->addFunction("concat");
  arrayType_->addFunction("join");
  arrayType_->addFunction("length");
  arrayType_->addFunction("pop");
  arrayType_->addFunction("push");
  arrayType_->addFunction("shift");
  arrayType_->addFunction("unshift");

  addTypeObject(CJToken::Type::Array, arrayType_);

  //------

  // Global
  setProperty("alert"        , CJValueP(new CJAlertFunction(this)));
  setProperty("setInterval"  , CJValueP(new CJSetInterval  (this)));
  setProperty("clearInterval", CJValueP(new CJClearInterval(this)));

  setProperty("undefined", CJValueP(new CJUndefined(this)));

  //------

  CJDictionary *objectDictionary = new CJDictionary(this);

  setProperty("Object", CJValueP(objectDictionary));

  objectDictionary->setProperty(this, "getOwnPropertyNames",
    new CJObjectFunction(this, "getOwnPropertyNames"));

  //------

  // Math Dictionary
  // TODO: mark as internal ? different behavior than user dictionary
  CJDictionary *mathDictionary = new CJDictionary(this);

  setProperty("Math", CJValueP(mathDictionary));

  mathDictionary->setProperty(this, "E"    , exp(1) );
  mathDictionary->setProperty(this, "LN2"  , log(2) );
  mathDictionary->setProperty(this, "LN10" , log(10));
  mathDictionary->setProperty(this, "PI"   , M_PI   );
  mathDictionary->setProperty(this, "SQRT2", sqrt(2));

  mathDictionary->setProperty(this, "abs"  , new CJRealFunction (this, "abs"  , fabs ));
  mathDictionary->setProperty(this, "acos" , new CJRealFunction (this, "acos" , acos ));
  mathDictionary->setProperty(this, "asin" , new CJRealFunction (this, "asin" , asin ));
  mathDictionary->setProperty(this, "atan" , new CJRealFunction (this, "atan" , atan ));
  mathDictionary->setProperty(this, "atan2", new CJReal2Function(this, "atan2", atan2));
  mathDictionary->setProperty(this, "ceil" , new CJRealFunction (this, "ceil" , ceil ));
  mathDictionary->setProperty(this, "cos"  , new CJRealFunction (this, "cos"  , cos  ));
  mathDictionary->setProperty(this, "exp"  , new CJRealFunction (this, "exp"  , exp  ));
  mathDictionary->setProperty(this, "floor", new CJRealFunction (this, "floor", floor));
  mathDictionary->setProperty(this, "log"  , new CJRealFunction (this, "log"  , log  ));
  mathDictionary->setProperty(this, "pow"  , new CJReal2Function(this, "pow"  , pow  ));
  mathDictionary->setProperty(this, "round", new CJRealFunction (this, "round", round));
  mathDictionary->setProperty(this, "sin"  , new CJRealFunction (this, "sin"  , sin  ));
  mathDictionary->setProperty(this, "sqrt" , new CJRealFunction (this, "sqrt" , sqrt ));
  mathDictionary->setProperty(this, "tan"  , new CJRealFunction (this, "tan"  , tan  ));

  //------

  // document (Variable lowercase)
  documentType_ = CJObjectTypeP(new CJDocumentType);

  setProperty("document", CJValueP(new CJDocumentObject(this)));

  // console (Variable lowercase)
  consoleType_ = CJObjectTypeP(new CJConsoleType);

  setProperty("console", CJValueP(new CJConsoleObject(this)));
}

CJScopeP
CJavaScript::
createScope(const std::string &name)
{
  CJScopeP scope = CJScopeP(new CJScope(this, name));

  scope_->addScope(scope);

  return scope;
}

void
CJavaScript::
deleteScope(const std::string &name)
{
  scope_->removeScope(name);
}

void
CJavaScript::
startScope(CJScopeP scope)
{
  scopeStack_.push_back(scope_);

  scope->setParent(scope_.get());

  scope_ = scope;
}

CJScopeP
CJavaScript::
endScope()
{
  CJScopeP scope = scope_;

  scope_ = scopeStack_.back();

  scopeStack_.pop_back();

  scope->setParent(0);

  return scope;
}

std::vector<std::string>
CJavaScript::
getFunctionNames() const
{
  return scope_->getFunctionNames();
}

std::vector<std::string>
CJavaScript::
getVariableNames() const
{
  return scope_->getVariableNames();
}

void
CJavaScript::
startBlock(CJExecBlockP block)
{
  blockStack_.push_back(block_);

  block_ = block;

  block_->initExec();
}

CJExecBlockP
CJavaScript::
endBlock()
{
  CJExecBlockP block = block_;

  block_ = blockStack_.back();

  blockStack_.pop_back();

  if      (block->type() == CJExecBlock::Type::Sequential) {
    if (block->isBreakFlag())
      block_->setBreakFlag(true);

    if (block->isContinueFlag())
      block_->setContinueFlag(true);

    if (block->getRetVal())
      block_->setRetVal(block->getRetVal());
  }
  else if (block->type() == CJExecBlock::Type::Iterative) {
    if (block->getRetVal())
      block_->setRetVal(block->getRetVal());
  }

  return block;
}

void
CJavaScript::
setProperty(const std::string &name, CJValueP value)
{
  scope_->setProperty(name, value);
}

//---

CJValueP
CJavaScript::
lookupValue(const Identifiers &identifiers)
{
  CJScope *scope = currentScope().get();

  CJValueP value = lookupValue(scope, identifiers);

  while (! value && scope->getParent()) {
    scope = scope->getParent();

    value = lookupValue(scope, identifiers);
  }

  return value;
}

CJValueP
CJavaScript::
lookupValue(CJScope *scope, const Identifiers &identifiers)
{
  int len = identifiers.size();

  if (len == 0)
    return CJValueP();

  //---

  if (len > 1) {
    CJScopeP scope1 = scope->lookupScope(identifiers[0]->name());

    if (scope1) {
      Identifiers identifiers1;

      for (int i = 1; i < len; ++i)
        identifiers1.push_back(identifiers[i]);

      return lookupValue(scope1.get(), identifiers1);
    }

    CJValueP varValue = scope->lookupProperty(identifiers[0]->name());

    if (! varValue)
      return CJValueP();

    Identifiers identifiers1;

    for (int i = 1; i < len; ++i)
      identifiers1.push_back(identifiers[i]);

    return lookupValue(varValue, identifiers1);
  }

  //---

  CJValueP value;

  CJValueP varValue = scope->lookupProperty(identifiers[0]->name());

  if (varValue) {
    value = varValue;

    if (! value)
      value = CJValueP(new CJUndefined(this));
  }

  return value;
}

CJValueP
CJavaScript::
lookupValue(CJValueP value, const Identifiers &identifiers)
{
  int len = identifiers.size();

  CJValueP value1;

  if (value->type() == CJToken::Type::Dictionary) {
    CJDictionary *dict = value->cast<CJDictionary>();

    value1 = dict->lookupProperty(identifiers[0]->name());
  }
  else {
    CJFunctionP fn = getTypeFunction(value->valueType(), identifiers[0]->name());

    if      (fn) {
      CJFunction::Values values;

      values.push_back(value);

      value1 = fn->exec(this, values);
    }
    else if (value->valueType()->type() == CJToken::Type::Object) {
      CJObject *obj = value->dup(this)->cast<CJObject>();

      CJObjectValue *value = new CJObjectValue(this, CJObjectP(obj), identifiers[0]->name());

      return CJValueP(value);
    }
    else {
      errorMsg("Function not found : " + identifiers[0]->name());

      return CJValueP();
    }
  }

  if (len > 1) {
    Identifiers identifiers1;

    for (int i = 1; i < len; ++i)
      identifiers1.push_back(identifiers[i]);

    return lookupValue(value1, identifiers1);
  }

  return value1;
}

//---

CJLValueP
CJavaScript::
lookupLValue(const Identifiers &identifiers)
{
  CJScope *scope = currentScope().get();

  CJLValueP lvalue = lookupLValue(scope, identifiers);

  while (! lvalue && scope->getParent()) {
    scope = scope->getParent();

    lvalue = lookupLValue(scope, identifiers);
  }

  return lvalue;
}

CJLValueP
CJavaScript::
lookupLValue(CJScope *scope, const Identifiers &identifiers)
{
  int len = identifiers.size();

  if (len == 0)
    return CJLValueP();

  //---

  if (len > 1) {
    CJScopeP scope1 = scope->lookupScope(identifiers[0]->name());

    if (scope1) {
      Identifiers identifiers1;

      for (int i = 1; i < len; ++i)
        identifiers1.push_back(identifiers[i]);

      return lookupLValue(scope1.get(), identifiers1);
    }

    CJValueP varValue = scope->lookupProperty(identifiers[0]->name());

    if (! varValue)
      return CJLValueP();

    Identifiers identifiers1;

    for (int i = 1; i < len; ++i)
      identifiers1.push_back(identifiers[i]);

    return lookupLValue(varValue, identifiers1);
  }

  //---

  CJLValueP lvalue;

  CJValueP varValue = scope->lookupProperty(identifiers[0]->name());

  if (varValue)
    return CJLValueP(new CJScopeValue(this, scope, identifiers[0]->name()));

  return CJLValueP();
}

CJLValueP
CJavaScript::
lookupLValue(CJValueP value, const Identifiers &identifiers)
{
  int len = identifiers.size();

  if (len > 1) {
    CJValueP rvalue;

    if (value->type() == CJToken::Type::Dictionary) {
      CJDictionary *dict = value->cast<CJDictionary>();

      rvalue = dict->lookupProperty(identifiers[0]->name());
    }
    else {
      CJFunctionP fn = getTypeFunction(value->valueType(), identifiers[0]->name());

      if (! fn) {
        errorMsg("Function not found : " + identifiers[0]->name());
        return CJLValueP();
      }

      CJFunction::Values values;

      values.push_back(value);

      rvalue = fn->exec(this, values);
    }

    Identifiers identifiers1;

    for (int i = 1; i < len; ++i)
      identifiers1.push_back(identifiers[i]);

    return lookupLValue(rvalue, identifiers1);
  }
  else {
    CJLValueP lvalue;

    if (value->type() == CJToken::Type::Dictionary) {
      CJDictionary *dict = value->cast<CJDictionary>();

      CJDictionaryRef *dref = new CJDictionaryRef(this, dict, identifiers[0]->name());

      return CJLValueP(dref);
    }
    else {
      //CJFunctionP fn = getTypeFunction(value->valueType(), identifiers[0]->name());

      //if (! fn) {
      //  errorMsg("Function not found : " + identifiers[0]->name());
      //  return CJValueP();
      //}

      //CJFunction::Values values;

      //values.push_back(value);

      //lvalue = fn->exec(this, values);
    }

    return lvalue;
  }
}

//---

#if 0
CJValueP
CJavaScript::
lookupFunction(CJScope *scope, const Identifiers &identifiers)
{
  int len = identifiers.size();

  CJValueP fn;

  for (int i = 0; i < len; ++i) {
    if (i < len - 1) {
      CJScopeP scope1 = scope->lookupScope(identifiers[i]->name());

      if (scope1) {
        scope = scope1.get();
        continue;
      }

      CJValueP varValue = scope->lookupProperty(identifiers[i]->name());

      if (varValue) {
        ++i;

        if (varValue->type() == CJToken::Type::Dictionary) {
          CJDictionary *dict = varValue->cast<CJDictionary>();

          CJValueP value = dict->lookupProperty(identifiers[i]->name());

          while (value && value->type() == CJToken::Type::Dictionary && i < len - 1) {
            ++i;

            dict = value->cast<CJDictionary>();

            value = dict->lookupProperty(identifiers[i]->name());
          }

          if (i < len - 1)
            errorMsg("Invalid value named index");

          if (value->type() != CJToken::Type::Function) {
            errorMsg("Dictionary value not a function");
            break;
          }

          fn = value;
        }
      }
      else {
        return CJValueP();
      }
    }
    else {
      fn = scope->lookupFunction(identifiers[i]->name());
    }
  }

  return fn;
}
#endif

CJLValueP
CJavaScript::
lookupProperty(const Identifiers &identifiers, bool create)
{
  CJScope *scope = currentScope().get();

  CJLValueP varValue = lookupProperty(scope, identifiers, create);

  while (! varValue && scope->getParent()) {
    scope = scope->getParent();

    varValue = lookupProperty(scope, identifiers, create);
  }

  return varValue;
}

CJLValueP
CJavaScript::
lookupProperty(CJScope *scope, const Identifiers &identifiers, bool create)
{
  int len = identifiers.size();

  for (int i = 0; i < len; ++i) {
    if (i < len - 1) {
      CJScopeP scope1 = scope->lookupScope(identifiers[i]->name());

      if (scope1) {
        scope = scope1.get();
        continue;
      }

      CJValueP varValue = scope->lookupProperty(identifiers[i]->name());

      if (varValue) {
        ++i;

        if (varValue->type() == CJToken::Type::Dictionary) {
          CJDictionary *dict = varValue->cast<CJDictionary>();

          CJValueP value = dict->lookupProperty(identifiers[i]->name());

          while (value && value->type() == CJToken::Type::Dictionary && i < len - 1) {
            ++i;

            dict = value->cast<CJDictionary>();

            value = dict->lookupProperty(identifiers[i]->name());
          }

          if (i < len - 1)
            errorMsg("Invalid value named index");

          if (value || create)
            return CJLValueP(new CJDictValue(this, dict, identifiers[i]->name()));
        }
      }

      return CJLValueP();
    }
    else {
      CJValueP varValue = scope->lookupProperty(identifiers[i]->name());

      if (varValue || create)
        return CJLValueP(new CJScopeValue(this, scope, identifiers[i]->name()));
    }
  }

  return CJLValueP();
}

bool
CJavaScript::
deleteProperty(const Identifiers &identifiers)
{
  CJScope *scope = currentScope().get();

  if (deleteProperty(scope, identifiers))
    return true;

  while (scope->getParent()) {
    scope = scope->getParent();

    if (deleteProperty(scope, identifiers))
      return true;
  }

  return false;
}

bool
CJavaScript::
deleteProperty(CJScope *scope, const Identifiers &identifiers)
{
  int len = identifiers.size();

  for (int i = 0; i < len; ++i) {
    if (i < len - 1) {
      CJScopeP scope1 = scope->lookupScope(identifiers[i]->name());

      if (scope1) {
        scope = scope1.get();
        continue;
      }

      CJValueP varValue = scope->lookupProperty(identifiers[i]->name());

      if (varValue) {
        ++i;

        if (varValue->type() == CJToken::Type::Dictionary) {
          CJDictionary *dict = varValue->cast<CJDictionary>();

          CJValueP value = dict->lookupProperty(identifiers[i]->name());

          while (value && value->type() == CJToken::Type::Dictionary && i < len - 1) {
            ++i;

            dict = value->cast<CJDictionary>();

            value = dict->lookupProperty(identifiers[i]->name());
          }

          if (i < len - 1)
            errorMsg("Invalid value named index");

          if (dict) {
            dict->deleteProperty(identifiers[i]->name());
            return true;
          }
        }
      }
    }
    else {
      scope->deleteProperty(identifiers[i]->name());

      return true;
    }
  }

  return false;
}

CJavaScript::FunctionValuePair
CJavaScript::
variableFunction(const Identifiers &identifiers)
{
  CJScope *scope = currentScope().get();

  auto fv = variableFunction(scope, identifiers);

  while (! fv.first && scope->getParent()) {
    scope = scope->getParent();

    fv = variableFunction(scope, identifiers);
  }

  return fv;
}

CJavaScript::FunctionValuePair
CJavaScript::
variableFunction(CJScope *scope, const Identifiers &identifiers)
{
  int len = identifiers.size();

  int i = 0;

  while (i < len - 1) {
    CJScopeP scope1 = scope->lookupScope(identifiers[i]->name());

    if (! scope1)
      break;

    scope = scope1.get();

    ++i;
  }

  //---

  CJValueP varValue;

  if (i < len - 1) {
    varValue = scope->lookupProperty(identifiers[i]->name());

    if (! varValue)
      return FunctionValuePair(0, 0);

    ++i;
  }

  if (! varValue)
    return FunctionValuePair(0, 0);

  //---

  CJValueP value = varValue;

  while (value && i < len - 1) {
    CJFunctionP fn = getTypeFunction(value->valueType(), identifiers[i]->name());

    if (! fn)
      return FunctionValuePair(0, 0);

    CJFunction::Values values;

    values.push_back(value);

    value = fn->exec(this, values);

    ++i;
  }

  CJFunctionP fn;

  if (value && i < len)
    fn = getTypeFunction(value->valueType(), identifiers[i]->name());

  return FunctionValuePair(fn, value);
}

CJValueP
CJavaScript::
setVariable(const Identifiers &identifiers, CJValueP value)
{
  CJLValueP varValue = lookupProperty(identifiers, /*create*/true);

  if (! varValue)
    return CJValueP();

  varValue->setValue(value);

  return varValue;
}

void
CJavaScript::
addTypeObject(CJToken::Type type, CJObjectTypeP obj)
{
  typeObject_[type] = obj;
}

CJFunctionP
CJavaScript::
getTypeFunction(CJObjectTypeP type, const std::string &name) const
{
  CJavaScript *th = const_cast<CJavaScript *>(this);

  if (type->type() == CJToken::Type::Object) {
    if (type->hasFunction(name)) {
      CJObjectFunction *fn = new CJObjectFunction(th, name);

      return CJFunctionP(fn);
    }
  }

  auto p1 = typeObject_.find(type->type());

  if (p1 != typeObject_.end()) {
    if (type->hasFunction(name)) {
      CJTypeObjectFunction *fn = new CJTypeObjectFunction(th, (*p1).second, name);

      return CJFunctionP(fn);
    }
  }

  return CJFunctionP();
}

void
CJavaScript::
loadFile(const std::string &filename)
{
  CFile file(filename);

  std::string str;

  std::string line;

  while (file.readLine(line)) {
    line = CStrUtil::stripSpaces(line);

    if (line.empty())
      continue;

    if (! str.empty())
      str += "\n";

    str += line;
  }

  loadString(str);
}

void
CJavaScript::
loadString(const std::string &str)
{
  tokens_.clear();

  parseString(str);

  if (isParseDebug()) {
    for (const auto &t : tokens_)
      std::cout << *t << std::endl;
  }
}

void
CJavaScript::
parseString(const std::string &str)
{
  CStrParse parse(str);

  parse.skipSpace();

  while (! parse.eof()) {
    CJToken::Type lastType = lastTokenType();

    bool allowUnary = false;

    if (lastType == CJToken::Type::None || lastType == CJToken::Type::Operator)
      allowUnary = true;

    if      (parse.isAlpha() || parse.isChar('_')) {
      readIdentifier(parse);
    }
    else if (parse.isDigit()) {
      readNumber(parse);
    }
    else if (parse.isOneOf("+-.") && parse.isDigitAt(1) && allowUnary) {
      readNumber(parse);
    }
    else if (parse.isString("//")) {
      parse.skipChars(2);

      while (! parse.eof() && ! parse.isChar('\n'))
        parse.skipChar();
    }
    else if (parse.isString("/*")) {
      parse.skipChars(2);

      while (! parse.eof() && ! parse.isString("*/"))
        parse.skipChar();

      if (parse.isString("*/"))
        parse.skipChars(2);
    }
    else if (parse.isOneOf("{}()[].;,<>=!+-*%/<>=&|^~?:")) {
      readOperator(parse, allowUnary);
    }
    else if (parse.isChar('"')) {
      readDoubleString(parse);
    }
    else if (parse.isChar('\'')) {
      readSingleString(parse);
    }
    else if (parse.isString("typeof")) {
      CJOperator *op = new CJOperator(CJOperator::Type::TypeOf, 1,
        CJOperator::Associativty::Right, CJOperator::Ary::Unary);

      tokens_.push_back(CJTokenP(op));
    }
    else {
      errorMsg("Invalid text: '" + parse.getAt() + "'");
      break;
    }

    parse.skipSpace();
  }
}

CJValueP
CJavaScript::
exec()
{
  ExecData execData;

  execData.initExec(tokens_);

  interp(execData);

  etokens_ = execData.etokens();

  CJValueP value;

  for (auto &t : etokens_) {
    value = t->exec(this);

    if (isExecDebug()) {
      std::cout << "exec: " << *t << " = ";

      if (value)
        std::cout << *value << std::endl;
      else
        std::cout << "<undefined>" << std::endl;
    }
  }

  return value;
}

void
CJavaScript::
interp(ExecData &execData)
{
  execDataStack_.push_back(execData_);

  execData_ = &execData;

  while (! execData_->eof()) {
    CJTokenP token = execData_->token();

    CJToken::Type type = token->type();

    if      (type == CJToken::Type::Identifier) {
      CJExecExpressionList *exprList = interpExpressionList();

      if (! exprList) {
        std::ostringstream ss; ss << *token;
        errorMsg("Interp failed at token: " + ss.str());
        break;
      }

      execData_->addEToken(CJTokenP(exprList));
    }
    else if (type == CJToken::Type::Operator) {
      CJOperator *op = token->cast<CJOperator>();

      CJOperator::Type opType = op->type();

      if (opType == CJOperator::Type::Colon) {
        if (! execData_->isBlock()) {
          std::ostringstream ss; ss << *token;
          errorMsg("Interp failed at operator: " + ss.str());
          break;
        }

        execData_->next();

        //---

        CJValueP key = execData_->unstackEValue(this);

        if (! key) {
          errorMsg("Left hand side value missing for colon");
          break;
        }

        CJExecExpression *valueExpr = interpExpression();

        if (! valueExpr) {
          errorMsg("Missing expression after colon");
          break;
        }

        CJValueP value = valueExpr->exec(this);

        delete valueExpr;

        //---

        CJDictionary *dict = new CJDictionary(this);

        dict->setProperty(key->toString(), value);

        //---

        while (isOperator(execData_->token(), CJOperator::Type::Comma)) {
          execData_->next();

          CJExecExpression *keyExpr = interpExpression();

          if (! keyExpr) {
            errorMsg("Missing key expression for dictionary");
            delete dict;
            break;
          }

          CJValueP key = keyExpr->exec(this);

          delete keyExpr;

          //---

          if (! isOperator(execData_->token(), CJOperator::Type::Colon)) {
            errorMsg("Missing colon after key for dictionary");
            delete dict;
            break;
          }

          execData_->next();

          CJExecExpression *valueExpr = interpExpression();

          if (! valueExpr) {
            errorMsg("Missing key expression for dictionary");
            delete dict;
            break;
          }

          CJValueP value = valueExpr->exec(this);

          delete valueExpr;

          //---

          dict->setProperty(key->toString(), value);
        }

        execData_->addEToken(CJTokenP(dict));

        break;
      }

      //---

      if (opType == CJOperator::Type::Comma ||
          opType == CJOperator::Type::CloseRBracket ||
          opType == CJOperator::Type::CloseSBracket) {
        std::ostringstream ss; ss << *token;
        errorMsg("Interp failed at operator: " + ss.str());
        break;
      }

      CJExecExpressionList *exprList = interpExpressionList();

      if (! exprList) {
        std::ostringstream ss; ss << *token;
        errorMsg("Interp failed at token: " + ss.str());
        break;
      }

      execData_->addEToken(CJTokenP(exprList));
    }
    else if (type == CJToken::Type::Number || type == CJToken::Type::String ||
             type == CJToken::Type::Undefined || type == CJToken::Type::Null ||
             type == CJToken::Type::True || type == CJToken::Type::False) {
      CJExecExpressionList *exprList = interpExpressionList();

      if (! exprList) {
        std::ostringstream ss; ss << *token;
        errorMsg("Interp failed at token: " + ss.str());
        break;
      }

      execData_->addEToken(CJTokenP(exprList));
    }
    else if (type == CJToken::Type::Keyword) {
      CJKeyword *keyword = token->cast<CJKeyword>();

      if      (keyword->type() == CJKeyword::Type::For) {
        execData_->next();

        CJExecFor *efor = interpFor();

        if (! efor) {
          errorMsg("Interp failed for 'for'");
          break;
        }

        execData_->addEToken(CJTokenP(efor));
      }
      else if (keyword->type() == CJKeyword::Type::If) {
        execData_->next();

        CJExecIf *eif = interpIf();

        if (! eif) {
          errorMsg("Interp failed for 'if'");
          break;
        }

        execData_->addEToken(CJTokenP(eif));
      }
      else if (keyword->type() == CJKeyword::Type::While) {
        execData_->next();

        CJExecWhile *ewhile = interpWhile();

        if (! ewhile) {
          errorMsg("Interp failed for 'while'");
          break;
        }

        execData_->addEToken(CJTokenP(ewhile));
      }
      else if (keyword->type() == CJKeyword::Type::Switch) {
        execData_->next();

        CJExecSwitch *eswitch = interpSwitch();

        if (! eswitch) {
          errorMsg("Interp failed for 'switch'");
          break;
        }

        execData_->addEToken(CJTokenP(eswitch));
      }
      else if (keyword->type() == CJKeyword::Type::Var) {
        execData_->next();

        CJExecVar *evar = interpVar();

        if (! evar) {
          errorMsg("Interp failed for 'var'");
          break;
        }

        execData_->addEToken(CJTokenP(evar));
      }
      else if (keyword->type() == CJKeyword::Type::New) {
        execData_->next();

        CJValue *value = interpNew();

        if (! value) {
          errorMsg("Interp failed for 'new'");
          break;
        }

        execData_->addEToken(CJTokenP(value));
      }
      else if (keyword->type() == CJKeyword::Type::Delete) {
        execData_->next();

        CJExecDelete *edelete = interpDelete();

        if (! edelete) {
          errorMsg("Interp failed for 'delete'");
          break;
        }

        execData_->addEToken(CJTokenP(edelete));
      }
      else if (keyword->type() == CJKeyword::Type::Break) {
        execData_->next();

        CJExecBreak *ebreak = new CJExecBreak;

        execData_->addEToken(CJTokenP(ebreak));
      }
      else if (keyword->type() == CJKeyword::Type::Continue) {
        execData_->next();

        CJExecContinue *econt = new CJExecContinue;

        execData_->addEToken(CJTokenP(econt));
      }
      else if (keyword->type() == CJKeyword::Type::Return) {
        execData_->next();

        CJExecReturn *eret = interpReturn();

        if (! eret) {
          errorMsg("Interp failed for 'return'");
          break;
        }

        execData_->addEToken(CJTokenP(eret));
      }
      else if (keyword->type() == CJKeyword::Type::Function) {
        execData_->next();

        CJUserFunction *efunction = interpUserFunction(/*named*/true);

        if (! efunction) {
          errorMsg("Interp failed for 'function'");
          break;
        }

        setProperty(efunction->name(), CJValueP(efunction));
      }
      else if (keyword->type() == CJKeyword::Type::Try) {
        execData_->next();

        CJExecTry *etry = interpTry();

        if (! etry) {
          errorMsg("Interp failed for 'try'");
          break;
        }

        execData_->addEToken(CJTokenP(etry));
      }
      else {
        errorMsg("Interp failed at keyword: " + keyword->name());
        break;
      }
    }
    else {
      std::stringstream ss; ss << *token;
      errorMsg("Interp failed at token: " + ss.str());
      break;
    }
  }

  execData_ = execDataStack_.back();

  execDataStack_.pop_back();
}

CJExecFunction *
CJavaScript::
interpFunction()
{
  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open round bracket");
    return 0;
  }

  execData_->next();

  CJExecFunction *func = new CJExecFunction;

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    CJExecExpressionList *exprList = interpExpressionList();

    if (! exprList) {
      delete func;
      return 0;
    }

    func->setExprList(CJExecExpressionListP(exprList));

    if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
      errorMsg("Missing close round bracket");
      delete func;
      return 0;
    }

    execData_->next();
  }
  else {
    execData_->next();
  }

  return func;
}

CJExecFor *
CJavaScript::
interpFor()
{
  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open round bracket");
    return 0;
  }

  execData_->next();

  CJExecFor *efor = new CJExecFor;

  CJTokenP token = execData_->token();

  bool isIn = false;

  if (isKeyword(token, CJKeyword::Type::Var)) {
    execData_->next();

    if (isInterpForIn()) {
      CJExecIdentifiers *identifiers = interpIdentfiers();

      if (! identifiers) {
        errorMsg("Missing variable names for for in");
        delete efor;
        return 0;
      }

      efor->setIdentifiers(CJExecIdentifiersP(identifiers), /*var*/true);

      isIn = true;
    }
    else {
      CJExecVar *evar = interpVar();

      if (! evar) {
        errorMsg("Interp failed for 'var'");
        delete efor;
        return 0;
      }

      efor->setVar(CJExecVarP(evar));
    }
  }
  else {
    if (isInterpForIn()) {
      CJExecIdentifiers *identifiers = interpIdentfiers();

      if (! identifiers) {
        errorMsg("Missing variable names for for in");
        delete efor;
        return 0;
      }

      efor->setIdentifiers(CJExecIdentifiersP(identifiers), /*var*/true);

      isIn = true;
    }
    else {
      CJExecExpressionList *exprList1 = interpExpressionList();

      if (! exprList1) {
        delete efor;
        return 0;
      }

      efor->setExprList1(CJExecExpressionListP(exprList1));
    }
  }

  if (isIn) {
    token = execData_->token();

    if (! isKeyword(token, CJKeyword::Type::In)) {
      errorMsg("Missing in for for in");
      delete efor;
      return 0;
    }

    execData_->next();

    CJExecExpression *expr = interpExpression();

    if (! expr) {
      delete efor;
      return 0;
    }

    efor->setInExpr(CJExecExpressionP(expr));
  }
  else {
    CJExecExpressionList *exprList2 = interpExpressionList();

    if (! exprList2) {
      delete efor;
      return 0;
    }

    efor->setExprList2(CJExecExpressionListP(exprList2));

    CJExecExpressionList *exprList3 = interpExpressionList();

    if (! exprList3) {
      delete efor;
      return 0;
    }

    efor->setExprList3(CJExecExpressionListP(exprList3));
  }

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg("Missing close round bracket");
    delete efor;
    return 0;
  }

  execData_->next();

  if (! isExecOperator(CJOperator::Type::OpenBrace)) {
    errorMsg("Missing open brace");
    delete efor;
    return 0;
  }

  CJExecBlock *block = interpBlock(CJExecBlock::Type::Iterative);

  if (! block) {
    delete efor;
    return 0;
  }

  efor->setBlock(CJExecBlockP(block));

  if (isInterpDebug()) {
    std::cerr << "interpFor: " << *efor << std::endl;
  }

  return efor;
}

bool
CJavaScript::
isInterpForIn()
{
  int pos = execData_->pos();

  CJTokenP token = execData_->token();

  if (token->type() != CJToken::Type::Identifier)
    return false;

  execData_->next();

  token = execData_->token();

  if (! isKeyword(token, CJKeyword::Type::In)) {
    execData_->setPos(pos);
    return false;
  }

  execData_->setPos(pos);

  return true;
}

CJExecQuestion *
CJavaScript::
interpQuestion(CJExecExpression *bexpr)
{
  CJExecQuestion *equestion = new CJExecQuestion(CJExecExpressionP(bexpr));

  CJExecExpression *expr1 = interpExpression();

  if (! expr1) {
    delete equestion;
    return 0;
  }

  equestion->setExpr1(CJExecExpressionP(expr1));

  if (! isExecOperator(CJOperator::Type::Colon)) {
    errorMsg("Missing colon for ?:");
    delete equestion;
    return 0;
  }

  execData_->next();

  CJExecExpression *expr2 = interpExpression();

  if (! expr2) {
    delete equestion;
    return 0;
  }

  equestion->setExpr2(CJExecExpressionP(expr2));

  if (isInterpDebug()) {
    std::cerr << "interpQuestion: " << *equestion << std::endl;
  }

  return equestion;
}

CJExecIf *
CJavaScript::
interpIf()
{
  CJExecIf *eif = new CJExecIf;

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open bracket for if");
    delete eif;
    return 0;
  }

  execData_->next();

  CJExecExpressionList *exprList = interpExpressionList();

  if (! exprList) {
    delete eif;
    return 0;
  }

  eif->setExprList(CJExecExpressionListP(exprList));

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg("Missing close bracket for if");
    delete eif;
    return 0;
  }

  execData_->next();

  CJExecBlock *ifBlock = interpBlock(CJExecBlock::Type::Sequential);

  if (! ifBlock) {
    delete eif;
    return 0;
  }

  eif->setIfBlock(CJExecBlockP(ifBlock));

  CJTokenP token = execData_->token();

  if (isKeyword(token, CJKeyword::Type::Else)) {
    execData_->next();

    CJExecBlock *elseBlock = interpBlock(CJExecBlock::Type::Sequential);

    if (! elseBlock) {
      delete eif;
      return 0;
    }

    eif->setElseBlock(CJExecBlockP(elseBlock));
  }

  if (isInterpDebug()) {
    std::cerr << "interpIf: " << *eif << std::endl;
  }

  return eif;
}

CJExecWhile *
CJavaScript::
interpWhile()
{
  CJExecWhile *ewhile = new CJExecWhile;

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open bracket for while");
    delete ewhile;
    return 0;
  }

  execData_->next();

  CJExecExpressionList *exprList = interpExpressionList();

  if (! exprList) {
    delete ewhile;
    return 0;
  }

  ewhile->setExprList(CJExecExpressionListP(exprList));

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg("Missing close bracket for while");
    delete ewhile;
    return 0;
  }

  execData_->next();

  CJExecBlock *block = interpBlock(CJExecBlock::Type::Iterative);

  if (! block) {
    delete ewhile;
    return 0;
  }

  ewhile->setBlock(CJExecBlockP(block));

  if (isInterpDebug()) {
    std::cerr << "interpWhile: " << *ewhile << std::endl;
  }

  return ewhile;
}

CJExecSwitch *
CJavaScript::
interpSwitch()
{
  CJExecSwitch *eswitch = new CJExecSwitch;

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open bracket for switch");
    delete eswitch;
    return 0;
  }

  execData_->next();

  CJExecExpressionList *exprList = interpExpressionList();

  if (! exprList) {
    delete eswitch;
    return 0;
  }

  eswitch->setExprList(CJExecExpressionListP(exprList));

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg("Missing close bracket for switch");
    delete eswitch;
    return 0;
  }

  execData_->next();

  //---

  if (! isExecOperator(CJOperator::Type::OpenBrace)) {
    errorMsg("Missing open brace");
    return 0;
  }

  execData_->next();

  CJExecBlock      *caseBlock = 0;
  CJExecExpression *caseExpr  = 0;
  CJExecBlock      *defBlock  = 0;

  int depth = 1;

  while (! execData_->eof()) {
    if      (isExecOperator(CJOperator::Type::OpenBrace))
      ++depth;
    else if (isExecOperator(CJOperator::Type::CloseBrace)) {
      --depth;

      if (depth == 0)
        break;
    }

    CJTokenP token = execData_->token();

    if (depth == 1) {
      if (token->type() == CJToken::Type::Keyword) {
        CJKeyword *keyword = token->cast<CJKeyword>();

        if      (keyword->type() == CJKeyword::Type::Case ||
                 keyword->type() == CJKeyword::Type::Default) {
          execData_->next();

          // add to previous case ?
          if      (caseExpr && caseBlock) {
            caseBlock->interp(this);

            eswitch->addCase(CJExecExpressionP(caseExpr), CJExecBlockP(caseBlock));

            caseExpr  = 0;
            caseBlock = 0;
          }
          else if (defBlock) {
            defBlock->interp(this);

            eswitch->setDefault(CJExecBlockP(defBlock));

            defBlock = 0;
          }

          if (keyword->type() == CJKeyword::Type::Case) {
            caseExpr = interpExpression();

            if (! caseExpr) {
              errorMsg("Invalid case value");
              return 0;
            }
          }

          if (! isExecOperator(CJOperator::Type::Colon)) {
            errorMsg("Missing colon for case expression");
            delete eswitch;
            return 0;
          }

          execData_->next();

          if (keyword->type() == CJKeyword::Type::Case)
            caseBlock = new CJExecBlock(CJExecBlock::Type::Sequential);
          else
            defBlock = new CJExecBlock(CJExecBlock::Type::Sequential);

          continue;
        }
        else if (keyword->type() == CJKeyword::Type::Break) {
          execData_->next();

          if      (caseExpr && caseBlock) {
            caseBlock->interp(this);

            eswitch->addCase(CJExecExpressionP(caseExpr), CJExecBlockP(caseBlock));

            caseExpr  = 0;
            caseBlock = 0;
          }
          else if (defBlock) {
            defBlock->interp(this);

            eswitch->setDefault(CJExecBlockP(defBlock));

            defBlock = 0;
          }

          if (isExecOperator(CJOperator::Type::SemiColon))
            execData_->next();

          continue;
        }
      }
    }

    if      (caseBlock)
      caseBlock->addToken(token);
    else if (defBlock)
      defBlock->addToken(token);
    else {
      errorMsg("Missing colon for case expression");
      delete eswitch;
      return 0;
    }

    execData_->next();
  }

  if (! isExecOperator(CJOperator::Type::CloseBrace)) {
    errorMsg("Missing close brace");
    delete eswitch;
    return 0;
  }

  execData_->next();

  //---

  if      (caseExpr && caseBlock) {
    caseBlock->interp(this);

    eswitch->addCase(CJExecExpressionP(caseExpr), CJExecBlockP(caseBlock));

    caseExpr  = 0;
    caseBlock = 0;
  }
  else if (defBlock) {
    defBlock->interp(this);

    eswitch->setDefault(CJExecBlockP(defBlock));

    defBlock = 0;
  }

  //---

  if (isInterpDebug()) {
    std::cerr << "interpSwitch: " << *eswitch << std::endl;
  }

  return eswitch;
}

CJExecVar *
CJavaScript::
interpVar()
{
  CJExecVar *var = new CJExecVar;

  while (! execData_->eof()) {
    CJTokenP token = execData_->token();

    if (token->type() != CJToken::Type::Identifier) {
      errorMsg("Missing identifier");
      delete var;
      return 0;
    }

    CJExecIdentifiers *identifiers = interpIdentfiers();

    token = execData_->token();

    if (isOperator(token, CJOperator::Type::Assign)) {
      execData_->next();

      token = execData_->token();

      if      (isOperator(token, CJOperator::Type::OpenSBracket)) {
        CJArray *array = interpArray();

        if (! array) {
          delete var;
          return 0;
        }

        var->setValue(CJValueP(array));
      }
      else if (isOperator(token, CJOperator::Type::OpenBrace)) {
        if (isInterpDictionary()) {
          CJDictionary *dict = interpDictionary();

          if (! dict) {
            delete var;
            return 0;
          }

          var->setValue(CJValueP(dict));
        }
        else {
          CJExecBlock *block = interpBlock(CJExecBlock::Type::Sequential);

          if (! block) {
            delete var;
            return 0;
          }

          var->setBlock(CJExecBlockP(block));
        }
      }
      else {
        CJExecExpressionList *exprList = interpExpressionList();

        if (! exprList) {
          delete var;
          return 0;
        }

        var->setExprList(CJExecExpressionListP(exprList));
      }
    }

    var->setIdentifiers(CJExecIdentifiersP(identifiers));

    if (! isOperator(token, CJOperator::Type::Comma)) {
      break;
    }

    execData_->next();
  }

  if (isInterpDebug()) {
    std::cerr << "interpVar: " << *var << std::endl;
  }

  return var;
}

CJArray *
CJavaScript::
interpArray()
{
  if (! isOperator(execData_->token(), CJOperator::Type::OpenSBracket))
    return 0;

  execData_->next();

  CJArray *array;

  if (! isOperator(execData_->token(), CJOperator::Type::CloseSBracket)) {
    CJExecExpressionList *exprList = interpExpressionList();

    if (! isOperator(execData_->token(), CJOperator::Type::CloseSBracket)) {
      errorMsg("Missing close square bracket for array");
      delete exprList;
      return 0;
    }

    execData_->next();

    CJExecExpressionList::Values values = exprList->getValues(this);

    array = new CJArray(this, values);

    delete exprList;
  }
  else {
    execData_->next();

    array = new CJArray(this);
  }

  return array;
}

bool
CJavaScript::
isInterpDictionary() const
{
  int pos = execData_->pos();

  if (! isOperator(execData_->token(), CJOperator::Type::OpenBrace))
    return false;

  execData_->next();

  if (isOperator(execData_->token(), CJOperator::Type::CloseBrace)) {
    execData_->setPos(pos);
    return true;
  }

  CJTokenP token = execData_->token();

  if      (token->type() == CJToken::Type::Identifier)
    execData_->next();
  else if (token->type() == CJToken::Type::Number)
    execData_->next();
  else {
    execData_->setPos(pos);
    return false;
  }

  if (! isOperator(execData_->token(), CJOperator::Type::Colon)) {
    execData_->setPos(pos);
    return false;
  }

  execData_->setPos(pos);

  return true;
}

CJDictionary *
CJavaScript::
interpDictionary()
{
  if (! isOperator(execData_->token(), CJOperator::Type::OpenBrace))
    return 0;

  execData_->next();

  CJDictionary *dict = 0;

  if (! isOperator(execData_->token(), CJOperator::Type::CloseBrace)) {
    dict = new CJDictionary(this);

    while (! execData_->eof()) {
      CJValueP key;

      CJTokenP token = execData_->token();

      if      (token->type() == CJToken::Type::Identifier)
        key = createStringValue(token->cast<CJIdentifier>()->name());
      else if (token->type() == CJToken::Type::Number)
        key = CJValueP(token.get()->cast<CJValue>()->dup(this));
      else {
        errorMsg("Invalid key value type");
        return 0;
      }

      execData_->next();

      if (! isOperator(execData_->token(), CJOperator::Type::Colon)) {
        errorMsg("Missing colon for key value");
        delete dict;
        return 0;
      }

      execData_->next();

      CJExecExpression *expr = interpExpression();

      if (! expr) {
        errorMsg("Invalid key value");
        delete dict;
        return 0;
      }

      CJValueP value = expr->exec(this);

      dict->setProperty(key->toString(), value);

      if (! isOperator(execData_->token(), CJOperator::Type::Comma))
        break;

      execData_->next();
    }

    if (! isOperator(execData_->token(), CJOperator::Type::CloseBrace)) {
      errorMsg("Missing close brace for dictionary");
      delete dict;
      return 0;
    }

     execData_->next();
  }
  else {
    execData_->next();

    dict = new CJDictionary(this);
  }

  return dict;
}

CJValue *
CJavaScript::
interpNew()
{
  CJTokenP token = execData_->token();

  if (token->type() != CJToken::Type::Identifier) {
    errorMsg("Missing identifier");
    return 0;
  }

  CJIdentifier *identifier = token->cast<CJIdentifier>();

  std::string typeName = identifier->name();

  execData_->next();

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open bracket for new");
    return 0;
  }

  execData_->next();

  CJExecExpressionList *exprList = interpExpressionList();

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg("Missing close bracket for new");
    delete exprList;
    return 0;
  }

  execData_->next();

  CJValue *value = 0;

  if      (typeName == "String") {
    CJString *str = new CJString(this);

    CJValueP value1 = exprList->exec(this);

    if (value1)
      str->setText(value1->toString());

    value = str;
  }
  else if (typeName == "Array") {
    CJExecExpressionList::Values values = exprList->getValues(this);

    CJArray *array;

    if (values.size() == 1 && values[0]->type() == CJValue::Type::Number) {
      long n = values[0]->toInteger();

      array = new CJArray(this, n);
    }
    else
      array = new CJArray(this, values);

    value = array;
  }
  else {
    errorMsg("Invalid type name");
    delete exprList;
    return 0;
  }

  delete exprList;

  if (isInterpDebug()) {
    std::cerr << "interpNew: " << *value << std::endl;
  }

  return value;
}

CJExecDelete *
CJavaScript::
interpDelete()
{
  CJExecIdentifiers *identifiers = interpIdentfiers();

  if (! identifiers)
    return 0;

  CJExecDelete *edelete = new CJExecDelete;

  edelete->setIdentifiers(CJExecIdentifiersP(identifiers));

  if (isInterpDebug()) {
    std::cerr << "interpDelete: " << *edelete << std::endl;
  }

  return edelete;
}

CJExecReturn *
CJavaScript::
interpReturn()
{
  CJExecExpression *expr = interpExpression();

  if (! expr)
    return 0;

  CJExecReturn *eret = new CJExecReturn(CJExecExpressionP(expr));

  if (isInterpDebug()) {
    std::cerr << "interpReturn: " << *eret << std::endl;
  }

  return eret;
}

CJExecTry *
CJavaScript::
interpTry()
{
  CJExecTry *etry = new CJExecTry;

  CJExecBlock *block1 = interpBlock(CJExecBlock::Type::Sequential);

  if (! block1) {
    delete etry;
    return 0;
  }

  etry->setTryBlock(CJExecBlockP(block1));

  CJTokenP token = execData_->token();

  if (! isKeyword(token, CJKeyword::Type::Catch)) {
    errorMsg("Missing catch for trye");
    delete etry;
    return 0;
  }

  execData_->next();

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open bracket for catch");
    return 0;
  }

  execData_->next();

  CJExecIdentifiers *identifiers = interpIdentfiers();

  if (! identifiers) {
    delete etry;
    return 0;
  }

  etry->setCatchIdentifiers(CJExecIdentifiersP(identifiers));

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg("Missing close bracket for catch");
    delete etry;
    return 0;
  }

  execData_->next();

  CJExecBlock *block2 = interpBlock(CJExecBlock::Type::Sequential);

  if (! block2) {
    delete etry;
    return 0;
  }

  etry->setCatchBlock(CJExecBlockP(block2));

  if (isInterpDebug()) {
    std::cerr << "interpTry: " << *etry << std::endl;
  }

  return etry;
}

CJUserFunction *
CJavaScript::
interpUserFunction(bool named)
{
  // optional function name
  std::string name;

  CJTokenP token = execData_->token();

  if (token->type() == CJToken::Type::Identifier) {
    name = token->cast<CJIdentifier>()->name();

    execData_->next();
  }
  else {
    if (named) {
      errorMsg("Missing identifier");
      return 0;
    }
  }

  //---

  // arguments
  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open bracket for function");
    return 0;
  }

  execData_->next();

  std::vector<std::string> args;

  token = execData_->token();

  while (token->type() == CJToken::Type::Identifier) {
    std::string arg = token->cast<CJIdentifier>()->name();

    args.push_back(arg);

    execData_->next();

    if (isExecOperator(CJOperator::Type::CloseRBracket))
      break;

    if (! isExecOperator(CJOperator::Type::Comma)) {
      errorMsg("Missing comma for function args");
      return 0;
    }

    execData_->next();

    token = execData_->token();

    if (token->type() != CJToken::Type::Identifier) {
      errorMsg("Missing arg after comma for function args");
      return 0;
    }
  }

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg("Missing close bracket for function");
    return 0;
  }

  execData_->next();

  //---

  // function block
  CJExecBlock *block = interpBlock(CJExecBlock::Type::Function);

  if (! block)
    return 0;

  //---

  CJUserFunction *userFn = new CJUserFunction(this, name, args, CJExecBlockP(block));

  if (isInterpDebug()) {
    std::cerr << "interpUserFunction: " << *userFn << std::endl;
  }

  return userFn;
}

CJExecExpressionList *
CJavaScript::
interpExpressionList()
{
  CJExecExpressionList::Expressions expressions;

  while (! execData_->eof()) {
    CJExecExpression *expr = interpExpression();

    if (! expr)
      return 0;

    CJExecExpressionP pexpr(expr);

    expressions.push_back(pexpr);

    if (! isExecOperator(CJOperator::Type::Comma))
      break;

    execData_->next();
  }

  //---

  CJExecExpressionList *exprList = new CJExecExpressionList;

  for (const auto &e : expressions)
    exprList->addExpression(e);

  //---

  if (isExecOperator(CJOperator::Type::SemiColon))
    execData_->next();

  if (isInterpDebug()) {
    if (exprList)
      std::cerr << "interpExpressionList: " << *exprList << std::endl;
  }

  return exprList;
}

CJExecExpression *
CJavaScript::
interpExpression()
{
  CJExecExpression *expr = new CJExecExpression;

  while (! execData_->eof()) {
    CJTokenP token = execData_->token();

    if      (token->type() == CJToken::Type::Identifier) {
      CJExecIndexExpression *iexpr = 0;

      CJExecIdentifiers *identifiers = interpIdentfiers();

      while (isExecOperator(CJOperator::Type::OpenSBracket)) {
        execData_->next();

        CJExecIndexExpression *iexpr1 = new CJExecIndexExpression;

        if (identifiers) {
          iexpr1->setIdentifiers(CJExecIdentifiersP(identifiers));

          identifiers = 0;
        }
        else {
          iexpr1->setExpression(CJExecExpressionP(iexpr));
        }

        iexpr = iexpr1;

        //----

        CJExecExpression *expr1 = interpExpression();

        if (expr1)
          iexpr->setIndexExpression(CJExecExpressionP(expr1));

        if (! isExecOperator(CJOperator::Type::CloseSBracket)) {
          errorMsg("Missing close square bracket");
          delete iexpr;
          return 0;
        }

        execData_->next();
      }

      if      (isExecOperator(CJOperator::Type::OpenRBracket)) {
        CJExecFunction *function = interpFunction();

        if (! function) {
          delete expr;
          return 0;
        }

        if (iexpr) {
          if (isInterpDebug()) {
            std::cerr << "interpIndexExpression: " << *iexpr << std::endl;
          }

          function->setExpression(CJExecExpressionP(iexpr));
        }
        else {
          function->setIdentifiers(CJExecIdentifiersP(identifiers));
        }

        if (isInterpDebug()) {
          std::cerr << "interpFunction: " << *function << std::endl;
        }

        expr->addToken(CJTokenP(function));
      }
      else {
        CJTokenP token1 = execData_->token();

        if (isOperator(token1, CJOperator::Type::Increment) ||
            isOperator(token1, CJOperator::Type::Decrement)) {
          execData_->next();

          CJExecIncrDecrExpression *incrDecr = new CJExecIncrDecrExpression;

          CJOperator *op1 = token1->cast<CJOperator>();

          if (iexpr) {
            if (isInterpDebug()) {
              std::cerr << "interpIndexExpression: " << *iexpr << std::endl;
            }

            incrDecr->setExpression(CJExecExpressionP(iexpr));
          }
          else {
            incrDecr->setIdentifiers(CJExecIdentifiersP(identifiers));
          }

          incrDecr->setOperator(CJOperatorP(op1->dup()));
          incrDecr->setPost    (true);

          expr->addToken(CJTokenP(incrDecr));
        }
        else {
          // add index expression or identifiers
          if (iexpr) {
            if (isInterpDebug()) {
              std::cerr << "interpIndexExpression: " << *iexpr << std::endl;
            }

            expr->addToken(CJTokenP(iexpr));
          }
          else {
            expr->addToken(CJTokenP(identifiers));
          }
        }
      }
    }
    else if (token->isValue()) {
      execData_->next();

      expr->addToken(token);
    }
    else if (token->type() == CJToken::Type::Operator) {
      CJOperator *op = token->cast<CJOperator>();

      CJOperator::Type opType = op->type();

      if      (op->isAssign()) {
        execData_->next();

        CJExecAssignExpression *assign = new CJExecAssignExpression;

        assign->setLExpression(CJExecExpressionP(expr));
        assign->setOperator   (CJOperatorP(op->dup()));

        CJExecExpression *expr1 = interpExpression();

        if (! expr1) {
          errorMsg("invalid assignment rhs expression");
          delete assign;
          return 0;
        }

        assign->setRExpression(CJExecExpressionP(expr1));

        return assign;
      }
      else if (opType == CJOperator::Type::Increment ||
               opType == CJOperator::Type::Decrement) {
        execData_->next();

        CJTokenP token1 = execData_->token();

        if      (token1->type() == CJToken::Type::Identifier) {
          CJExecIdentifiers *identifiers = interpIdentfiers();

          CJExecIncrDecrExpression *incrDecr = new CJExecIncrDecrExpression;

          incrDecr->setIdentifiers(CJExecIdentifiersP(identifiers));
          incrDecr->setOperator   (CJOperatorP(op->dup()));
          incrDecr->setPost       (false);

          expr->addToken(CJTokenP(incrDecr));
        }
        else {
          errorMsg("invalid increment/decrement rhs");
          return 0;
        }
      }
      else if (opType == CJOperator::Type::Question) {
        execData_->next();

        CJExecQuestion *equestion = interpQuestion(expr);

        if (! equestion) {
          errorMsg("Interp failed for '?'");
          break;
        }

        expr = new CJExecExpression;

        expr->addToken(CJTokenP(equestion));
      }
      else if (opType == CJOperator::Type::SemiColon) {
        return expr;
      }
      else if (opType == CJOperator::Type::Comma) {
        return expr;
      }
      else if (opType == CJOperator::Type::Colon) {
        return expr;
      }
      else if (opType == CJOperator::Type::OpenRBracket) {
        execData_->next();

        CJExecExpression *expr1 = interpExpression();

        if (expr1)
          expr->addToken(CJTokenP(expr1));

        if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
          errorMsg("Missing close round bracket");
          delete expr;
          return 0;
        }

        execData_->next();
      }
      else if (opType == CJOperator::Type::CloseRBracket) {
        return expr;
      }
      else if (opType == CJOperator::Type::OpenSBracket) {
        CJArray *array = interpArray();

        if (! array) {
          delete expr;
          return 0;
        }

        if (isExecOperator(CJOperator::Type::Scope)) {
          execData_->next();

          CJExecIdentifiers *identifiers = interpIdentfiers();

          CJExecFunction *function = interpFunction();

          if (! function) {
            delete expr;
            return 0;
          }

          expr->addToken(CJTokenP(array));

          function->setExpression (CJExecExpressionP(expr));
          function->setIdentifiers(CJExecIdentifiersP(identifiers));

          if (isInterpDebug()) {
            std::cerr << "interpFunction: " << *function << std::endl;
          }

          //---

          expr = new CJExecExpression;

          expr->addToken(CJTokenP(function));
        }
        else {
          expr->addToken(CJTokenP(array));
        }
      }
      else if (opType == CJOperator::Type::CloseSBracket) {
        return expr;
      }
      else if (opType == CJOperator::Type::OpenBrace) {
        if (isInterpDictionary()) {
          CJDictionary *dict = interpDictionary();

          if (! dict) {
            delete expr;
            return 0;
          }

          if (isExecOperator(CJOperator::Type::Scope)) {
            execData_->next();

            CJExecIdentifiers *identifiers = interpIdentfiers();

            CJExecFunction *function = interpFunction();

            if (! function) {
              delete expr;
              return 0;
            }

            expr->addToken(CJTokenP(dict));

            function->setExpression (CJExecExpressionP(expr));
            function->setIdentifiers(CJExecIdentifiersP(identifiers));

            if (isInterpDebug()) {
              std::cerr << "interpFunction: " << *function << std::endl;
            }

            //---

            expr = new CJExecExpression;

            expr->addToken(CJTokenP(function));
          }
          else {
            expr->addToken(CJTokenP(dict));
          }
        }
        else {
          CJExecBlock *block = interpBlock(CJExecBlock::Type::Iterative);

          if (! block) {
            delete expr;
            return 0;
          }

          expr->addToken(CJTokenP(block));
        }
      }
      else if (opType == CJOperator::Type::CloseBrace) {
        return expr;
      }
      else {
        execData_->next();

        expr->addToken(token);
      }
    }
    else if (token->type() == CJToken::Type::Keyword) {
      CJKeyword *keyword = token->cast<CJKeyword>();

      if      (keyword->type() == CJKeyword::Type::New) {
        execData_->next();

        CJValue *value = interpNew();

        if (! value) {
          errorMsg("Interp failed for 'new'");
          break;
        }

        expr->addToken(CJTokenP(value));
      }
      else if (keyword->type() == CJKeyword::Type::Function) {
        execData_->next();

        CJUserFunction *efunction = interpUserFunction(/*named*/false);

        if (! efunction) {
          errorMsg("Interp failed for 'function'");
          break;
        }

        CJValueP value(efunction);

        setProperty(efunction->name(), value);

        expr->addToken(value);
      }
      else {
        errorMsg("Interp failed at keyword: " + keyword->name());
        break;
      }
    }
    else {
      std::stringstream ss; ss << *token;
      errorMsg("Interp failed at token: " + ss.str());
      delete expr;
      return 0;
    }
  }

  if (isInterpDebug()) {
    std::cerr << "interpExpression: " << *expr << std::endl;
  }

  return expr;
}

CJExecIdentifiers *
CJavaScript::
interpIdentfiers()
{
  CJTokenP token = execData_->token();

  if (token->type() != CJToken::Type::Identifier)
    return 0;

  CJExecIdentifiers *identifiers = new CJExecIdentifiers;

  execData_->next();

  identifiers->addIdentifier(token->cast<CJIdentifier>());

  while (isExecOperator(CJOperator::Type::Scope)) {
    execData_->next();

    CJTokenP token = execData_->token();

    if (token->type() != CJToken::Type::Identifier) {
      delete identifiers;
      return 0;
    }

    execData_->next();

    identifiers->addIdentifier(token->cast<CJIdentifier>());
  }

  if (isInterpDebug()) {
    std::cerr << "interpIdentfiers: " << *identifiers << std::endl;
  }

  return identifiers;
}

CJExecBlock *
CJavaScript::
interpBlock(CJExecBlock::Type type)
{
  CJExecBlock *block = new CJExecBlock(type);

  if (isExecOperator(CJOperator::Type::OpenBrace)) {
    execData_->next();

    int depth = 1;

    while (! execData_->eof()) {
      if      (isExecOperator(CJOperator::Type::OpenBrace))
        ++depth;
      else if (isExecOperator(CJOperator::Type::CloseBrace)) {
        --depth;

        if (depth == 0)
          break;
      }

      CJTokenP token = execData_->token();

      block->addToken(token);

      execData_->next();
    }

    if (! isExecOperator(CJOperator::Type::CloseBrace)) {
      errorMsg("Missing close brace");
      delete block;
      return 0;
    }

    execData_->next();
  }
  else {
    while (! execData_->eof()) {
      if (isExecOperator(CJOperator::Type::SemiColon))
        break;

      CJTokenP token = execData_->token();

      block->addToken(token);

      execData_->next();
    }

    if (isExecOperator(CJOperator::Type::SemiColon))
      execData_->next();
  }

  block->interp(this);

  if (isInterpDebug()) {
    std::cerr << "interpBlock: " << *block << std::endl;
  }

  return block;
}

bool
CJavaScript::
isOperator(CJTokenP token, CJOperator::Type opType) const
{
  if (! token || token->type() != CJToken::Type::Operator)
    return false;

  CJOperator *op = token->cast<CJOperator>();

  return (op->type() == opType);
}

bool
CJavaScript::
isKeyword(CJTokenP token, CJKeyword::Type type) const
{
  if (! token || token->type() != CJToken::Type::Keyword)
    return false;

  CJKeyword *keyword = token->cast<CJKeyword>();

  return (keyword->type() == type);
}

bool
CJavaScript::
isExecOperator(CJOperator::Type opType) const
{
  if (execData_->eof())
    return false;

  CJTokenP token = execData_->token();

  if (token->type() != CJToken::Type::Operator)
    return false;

  CJOperator *op = token->cast<CJOperator>();

  return (op->type() == opType);
}

int
CJavaScript::
cmp(CJValueP value1, CJValueP value2)
{
  if      (value1->type() == CJValue::Type::String ||
           value2->type() == CJValue::Type::String) {
    std::string s1 = value1->toString();
    std::string s2 = value2->toString();

    if (s1 < s2) return -1;
    if (s1 > s2) return  1;

    return 0;
  }
  else if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
    double r1 = value1->cast<CJNumber>()->real();
    double r2 = value2->cast<CJNumber>()->real();

    if (r1 < r2) return -1;
    if (r1 > r2) return  1;

    return 0;
  }
  else {
    if (value1 < value2) return -1;
    if (value1 > value2) return  1;

    return 0;
  }
}

CJValueP
CJavaScript::
execBinaryOp(CJOperator::Type op, CJValueP value1, CJValueP value2)
{
  if (! value1 || ! value2)
    return CJValueP();

  switch (op) {
    case CJOperator::Type::Plus:
      if      (value1->type() == CJValue::Type::String ||
               value2->type() == CJValue::Type::String) {
        std::string s1 = value1->toString();
        std::string s2 = value2->toString();

        return createStringValue(s1 + s2);
      }
      else if (value1->type() == CJValue::Type::Number &&
               value2->type() == CJValue::Type::Number) {
        double r1 = value1->cast<CJNumber>()->real();
        double r2 = value2->cast<CJNumber>()->real();

        return CJValueP(createNumberValue(r1 + r2));
      }

      break;
    case CJOperator::Type::Minus:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        double r1 = value1->cast<CJNumber>()->real();
        double r2 = value2->cast<CJNumber>()->real();

        return CJValueP(createNumberValue(r1 - r2));
      }

      break;
    case CJOperator::Type::Times:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        double r1 = value1->cast<CJNumber>()->real();
        double r2 = value2->cast<CJNumber>()->real();

        return CJValueP(createNumberValue(r1 * r2));
      }

      break;
    case CJOperator::Type::Divide:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        double r1 = value1->cast<CJNumber>()->real();
        double r2 = value2->cast<CJNumber>()->real();

        if (r2 != 0)
          return CJValueP(createNumberValue(r1 / r2));
        else
          return CJValueP(createNumberValue(getNaN()));
      }

      break;
    case CJOperator::Type::Modulus:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        double r1 = value1->cast<CJNumber>()->real();
        double r2 = value2->cast<CJNumber>()->real();

        return CJValueP(createNumberValue(realModulus(r1, r2)));
      }

      break;

    case CJOperator::Type::Less:
      return createBoolValue(cmp(value1, value2) <  0);
    case CJOperator::Type::Greater:
      return createBoolValue(cmp(value1, value2) >  0);
    case CJOperator::Type::LessEqual:
      return createBoolValue(cmp(value1, value2) <= 0);
    case CJOperator::Type::GreaterEqual:
      return createBoolValue(cmp(value1, value2) >= 0);
    case CJOperator::Type::Equal:
      return createBoolValue(cmp(value1, value2) == 0);
    case CJOperator::Type::NotEqual:
      return createBoolValue(cmp(value1, value2) != 0);

    case CJOperator::Type::StrictEqual:
      return createBoolValue(value1->type() == value2->type() && cmp(value1, value2) == 0);
    case CJOperator::Type::StrictNotEqual:
      return createBoolValue(value1->type() != value2->type() || cmp(value1, value2) != 0);

    case CJOperator::Type::LogicalAnd: {
      bool i1 = value1->toBoolean();

      bool res = false;

      if (i1) {
        bool i2 = value2->toBoolean();

        res = (i1 && i2);
      }

      return CJValueP(createBoolValue(res));
    }

    case CJOperator::Type::LogicalOr: {
      bool i1 = value1->toBoolean();

      bool res = true;

      if (! i1) {
        bool i2 = value2->toBoolean();

        res = (i1 || i2);
      }

      return CJValueP(createBoolValue(res));
    }

    case CJOperator::Type::BitwiseAnd:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        long res = (i1 & i2);

        return CJValueP(createNumberValue(res));
      }

      break;
    case CJOperator::Type::BitwiseOr:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        long res = (i1 | i2);

        return CJValueP(createNumberValue(res));
      }

      break;
    case CJOperator::Type::BitwiseXor:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        long res = (i1 ^ i2);

        return CJValueP(createNumberValue(res));
      }

      break;

    case CJOperator::Type::LeftShift:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        long res = (i1 << i2);

        return CJValueP(createNumberValue(res));
      }

      break;
    case CJOperator::Type::RightShift:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        long res = (i1 >> i2);

        return CJValueP(createNumberValue(res));
      }

      break;
    case CJOperator::Type::UnsignedRightShift:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        ulong i1 = value1->toInteger();
        long  i2 = value2->toInteger();

        long res = (i1 >> i2);

        return CJValueP(createNumberValue(res));
      }

      break;

    default:
      assert(false);
      break;
  }

  return CJValueP();
}

CJValueP
CJavaScript::
execUnaryOp(CJOperator::Type op, CJValueP value)
{
  if (! value)
    return CJValueP();

  switch (op) {
    case CJOperator::Type::UnaryPlus: {
      double r = value->toReal();

      return CJValueP(createNumberValue(r));
    }
    case CJOperator::Type::UnaryMinus: {
      double r = value->toReal();

      return CJValueP(createNumberValue(-r));
    }

    case CJOperator::Type::Increment:
      if (value->type() == CJValue::Type::Number) {
        double r = value->cast<CJNumber>()->real();

        return CJValueP(createNumberValue(r + 1));
      }

      break;
    case CJOperator::Type::Decrement:
      if (value->type() == CJValue::Type::Number) {
        double r = value->cast<CJNumber>()->real();

        return CJValueP(createNumberValue(r - 1));
      }

      break;

    case CJOperator::Type::LogicalNot: {
      bool i = value->toBoolean();

      bool res = ! i;

      return CJValueP(createBoolValue(res));
    }

    case CJOperator::Type::BitwiseNot:
      if (value->type() == CJValue::Type::Number) {
        long i = value->toInteger();

        long res = ~ i;

        return CJValueP(createNumberValue(res));
      }

      break;

    default:
      assert(false);
      break;
  }

  return CJValueP();
}

void
CJavaScript::
readIdentifier(CStrParse &parse)
{
  std::string name;

  while (! parse.eof() && (parse.isAlnum() || parse.isChar('_'))) {
    name += parse.readChar();
  }

  if      (name == "null")
    tokens_.push_back(CJTokenP(new CJNull(this)));
  else if (name == "true")
    tokens_.push_back(CJTokenP(new CJTrue(this)));
  else if (name == "false")
    tokens_.push_back(CJTokenP(new CJFalse(this)));
  else {
    auto k = isKeyword(name);

    if (k.first)
      tokens_.push_back(CJTokenP(new CJKeyword(k.second)));
    else
      tokens_.push_back(CJTokenP(new CJIdentifier(name)));
  }
}

std::pair<bool, CJKeyword::Type>
CJavaScript::
isKeyword(const std::string &name) const
{
  typedef std::map<std::string,CJKeyword::Type> Keywords;

  static Keywords keywords;

  if (keywords.empty()) {
    keywords["break"    ] = CJKeyword::Type::Break;
    keywords["case"     ] = CJKeyword::Type::Case;
    keywords["catch"    ] = CJKeyword::Type::Catch;
    keywords["class"    ] = CJKeyword::Type::Class;
    keywords["const"    ] = CJKeyword::Type::Const;
    keywords["continue" ] = CJKeyword::Type::Continue;
    keywords["default"  ] = CJKeyword::Type::Default;
    keywords["delete"   ] = CJKeyword::Type::Delete;
    keywords["do"       ] = CJKeyword::Type::Do;
    keywords["else"     ] = CJKeyword::Type::Else;
    keywords["finally"  ] = CJKeyword::Type::Finally;
    keywords["for"      ] = CJKeyword::Type::For;
    keywords["function" ] = CJKeyword::Type::Function;
    keywords["if"       ] = CJKeyword::Type::If;
    keywords["in"       ] = CJKeyword::Type::In;
    keywords["new"      ] = CJKeyword::Type::New;
    keywords["return"   ] = CJKeyword::Type::Return;
    keywords["switch"   ] = CJKeyword::Type::Switch;
    keywords["this"     ] = CJKeyword::Type::This;
    keywords["throw"    ] = CJKeyword::Type::Throw;
    keywords["try"      ] = CJKeyword::Type::Try;
    keywords["var"      ] = CJKeyword::Type::Var;
    keywords["void"     ] = CJKeyword::Type::Void;
    keywords["while"    ] = CJKeyword::Type::While;
    keywords["with"     ] = CJKeyword::Type::With;
  }

  auto p = keywords.find(name);

  if (p != keywords.end())
    return std::pair<bool, CJKeyword::Type>(true, (*p).second);
  else
    return std::pair<bool, CJKeyword::Type>(false, CJKeyword::Type::None);
}

void
CJavaScript::
readNumber(CStrParse &parse)
{
  int sign = 1;

  if (parse.isOneOf("+-")) {
    char c = parse.readChar();

    if (c == '-')
      sign = -1;
  }

  //---

  int hexValue = 0; bool isHex = false;
  int octValue = 0; bool isOct = false;

  std::string realStr;

  // octal (0[0-7]*), hexadecimal (0[xX][0-F]*), or decimal (0.[0-9]*)
  if (parse.isChar('0')) {
    parse.skipChar();

    if (parse.isOneOf("xX")) {
      isHex = true;

      parse.skipChar();

      while (! parse.eof() && parse.isXDigit()) {
        char c = parse.readChar();

        hexValue = 16*hexValue + hexCharValue(c);
      }
    }
    else if (parse.isChar('.')) {
      realStr = parse.readChar();

      while (! parse.eof() && parse.isDigit()) {
        realStr += parse.readChar();
      }

      if (! parse.eof() && parse.isOneOf("eE")) {
        realStr += parse.readChar();

        if (! parse.eof() && parse.isOneOf("+-."))
          realStr += parse.readChar();

        while (! parse.eof() && parse.isDigit()) {
          realStr += parse.readChar();
        }
      }
    }
    else {
      isOct = true;

      while (! parse.eof() && parse.isODigit()) {
        char c = parse.readChar();

        octValue = 8*octValue + (c - '0');
      }
    }
  }
  else {
    while (! parse.eof() && parse.isDigit()) {
      realStr += parse.readChar();
    }

    if (! parse.eof() && parse.isChar('.')) {
      realStr += parse.readChar();

      while (! parse.eof() && parse.isDigit()) {
        realStr += parse.readChar();
      }
    }

    if (! parse.eof() && parse.isOneOf("eE")) {
      realStr += parse.readChar();

      if (! parse.eof() && parse.isOneOf("+-."))
        realStr += parse.readChar();

      while (! parse.eof() && parse.isDigit()) {
        realStr += parse.readChar();
      }
    }
  }

  CJValueP number;

  if      (isOct)
    number = createNumberValue(long(sign*octValue));
  else if (isHex)
    number = createNumberValue(long(sign*hexValue));
  else {
    const char *p;

    double real = strtod(realStr.c_str(), (char **) &p);

    number = createNumberValue(sign*real);
  }

  tokens_.push_back(number);
}

void
CJavaScript::
readOperator(CStrParse &parse, bool allowUnary)
{
  CJOperator::Type         type         = CJOperator::Type::None;
  int                      precedence   = 0;
  CJOperator::Associativty associativty = CJOperator::Associativty::Left;
  CJOperator::Ary          ary          = CJOperator::Ary::Binary;

  std::string name;

  char c = parse.readChar();

  switch (c) {
    case '{':
      type = CJOperator::Type::OpenBrace;
      ary  = CJOperator::Ary::None;
      break;
    case '}':
      type = CJOperator::Type::CloseBrace;
      ary  = CJOperator::Ary::None;
      break;
    case '(':
      type = CJOperator::Type::OpenRBracket;
      ary  = CJOperator::Ary::None;
      break;
    case ')':
      type = CJOperator::Type::CloseRBracket;
      ary  = CJOperator::Ary::None;
      break;
    case '[':
      type = CJOperator::Type::OpenSBracket;
      ary  = CJOperator::Ary::None;
      break;
    case ']':
      type = CJOperator::Type::CloseSBracket;
      ary  = CJOperator::Ary::None;
      break;
    case '.':
      type = CJOperator::Type::Scope;
      break;
    case ';':
      type = CJOperator::Type::SemiColon;
      ary  = CJOperator::Ary::None;
      break;
    case ',':
      type       = CJOperator::Type::Comma;
      precedence = 14;
      ary        = CJOperator::Ary::None;
      break;
    case '<':
      if      (parse.isChar('=')) {
        parse.skipChar();

        type       = CJOperator::Type::LessEqual;
        precedence = 5;
      }
      else if (parse.isChar('<')) {
        parse.skipChar();

        if (parse.isChar('=')) {
          parse.skipChar();

          type         = CJOperator::Type::LeftShiftAssign;
          precedence   = 13;
          associativty = CJOperator::Associativty::Right;
        }
        else {
          type       = CJOperator::Type::LeftShift;
          precedence = 4;
        }
      }
      else {
        type       = CJOperator::Type::Less;
        precedence = 5;
      }
      break;
    case '>':
      if (parse.isChar('=')) {
        parse.skipChar();

        type       = CJOperator::Type::GreaterEqual;
        precedence = 5;
      }
      else if (parse.isChar('>')) {
        parse.skipChar();

        if (parse.isChar('=')) {
          parse.skipChar();

          type         = CJOperator::Type::RightShiftAssign;
          precedence   = 13;
          associativty = CJOperator::Associativty::Right;
        }
        else if (parse.isChar('>')) {
          parse.skipChar();

          if (parse.isChar('=')) {
            parse.skipChar();

            type         = CJOperator::Type::UnsignedRightShiftAssign;
            precedence   = 13;
            associativty = CJOperator::Associativty::Right;
          }
          else {
            type       = CJOperator::Type::UnsignedRightShift;
            precedence = 4;
          }
        }
        else {
          type       = CJOperator::Type::RightShift;
          precedence = 4;
        }
      }
      else {
        type       = CJOperator::Type::Greater;
        precedence = 5;
      }
      break;
    case '=':
      if (parse.isChar('=')) {
        parse.skipChar();

        if (parse.isChar('=')) {
          parse.skipChar();

          type       = CJOperator::Type::StrictEqual;
          precedence = 6;
        }
        else {
          type       = CJOperator::Type::Equal;
          precedence = 6;
        }
      }
      else {
        type         = CJOperator::Type::Assign;
        precedence   = 13;
        associativty = CJOperator::Associativty::Right;
      }
      break;
    case '+':
      if      (parse.isChar('+')) {
        parse.skipChar();

        type         = CJOperator::Type::Increment;
        precedence   = 1;
        associativty = CJOperator::Associativty::Right;
        ary          = CJOperator::Ary::Unary;
      }
      else if (parse.isChar('=')) {
        parse.skipChar();

        type         = CJOperator::Type::PlusAssign;
        precedence   = 13;
        associativty = CJOperator::Associativty::Right;
      }
      else {
        if (allowUnary) {
          type         = CJOperator::Type::UnaryPlus;
          precedence   = 1;
          associativty = CJOperator::Associativty::Right;
          ary          = CJOperator::Ary::Unary;
        }
        else {
          type       = CJOperator::Type::Plus;
          precedence = 3;
        }
      }
      break;
    case '-':
      if      (parse.isChar('-')) {
        parse.skipChar();

        type         = CJOperator::Type::Decrement;
        precedence   = 1;
        associativty = CJOperator::Associativty::Right;
        ary          = CJOperator::Ary::Unary;
      }
      else if (parse.isChar('=')) {
        parse.skipChar();

        type         = CJOperator::Type::MinusAssign;
        precedence   = 13;
        associativty = CJOperator::Associativty::Right;
      }
      else {
        if (allowUnary) {
          type         = CJOperator::Type::UnaryMinus;
          precedence   = 1;
          associativty = CJOperator::Associativty::Right;
          ary          = CJOperator::Ary::Unary;
        }
        else {
          type       = CJOperator::Type::Minus;
          precedence = 3;
        }
      }
      break;
    case '*':
      if (parse.isChar('=')) {
        parse.skipChar();

        type         = CJOperator::Type::TimesAssign;
        precedence   = 13;
        associativty = CJOperator::Associativty::Right;
      }
      else {
        type       = CJOperator::Type::Times;
        precedence = 2;
      }
      break;
    case '/':
      if (parse.isChar('=')) {
        parse.skipChar();

        type         = CJOperator::Type::DivideAssign;
        precedence   = 13;
        associativty = CJOperator::Associativty::Right;
      }
      else {
        type       = CJOperator::Type::Divide;
        precedence = 2;
      }
      break;
    case '%':
      if (parse.isChar('=')) {
        parse.skipChar();

        type         = CJOperator::Type::ModulusAssign;
        precedence   = 13;
        associativty = CJOperator::Associativty::Right;
      }
      else {
        type       = CJOperator::Type::Modulus;
        precedence = 2;
      }
      break;
    case '!':
      if (parse.isChar('=')) {
        parse.skipChar();

        if (parse.isChar('=')) {
          parse.skipChar();

          type       = CJOperator::Type::StrictNotEqual;
          precedence = 6;
        }
        else {
          type       = CJOperator::Type::NotEqual;
          precedence = 6;
        }
      }
      else {
        type         = CJOperator::Type::LogicalNot;
        precedence   = 1;
        associativty = CJOperator::Associativty::Right;
        ary          = CJOperator::Ary::Unary;
      }
      break;
    case '^':
      if (parse.isChar('=')) {
        parse.skipChar();

        type         = CJOperator::Type::BitwiseXorAssign;
        precedence   = 13;
        associativty = CJOperator::Associativty::Right;
      }
      else {
        type       = CJOperator::Type::BitwiseXor;
        precedence = 8;
      }
      break;
    case '&':
      if      (parse.isChar('=')) {
        parse.skipChar();

        type         = CJOperator::Type::BitwiseAndAssign;
        precedence   = 13;
        associativty = CJOperator::Associativty::Right;
      }
      else if (parse.isChar('&')) {
        parse.skipChar();

        type       = CJOperator::Type::LogicalAnd;
        precedence = 10;
      }
      else {
        type       = CJOperator::Type::BitwiseAnd;
        precedence = 7;
      }
      break;
    case '|':
      if      (parse.isChar('=')) {
        parse.skipChar();

        type         = CJOperator::Type::BitwiseOrAssign;
        precedence   = 13;
        associativty = CJOperator::Associativty::Right;
      }
      else if (parse.isChar('|')) {
        parse.skipChar();

        type       = CJOperator::Type::LogicalOr;
        precedence = 11;
      }
      else {
        type       = CJOperator::Type::BitwiseOr;
        precedence = 9;
      }
      break;
    case '~':
      if (parse.isChar('=')) {
        parse.skipChar();

        type         = CJOperator::Type::BitwiseNotAssign;
        precedence   = 13;
        associativty = CJOperator::Associativty::Right;
      }
      else {
        type         = CJOperator::Type::BitwiseNot;
        precedence   = 1;
        associativty = CJOperator::Associativty::Right;
        ary          = CJOperator::Ary::Unary;
      }
      break;
    case '?':
      type         = CJOperator::Type::Question;
      precedence   = 12;
      associativty = CJOperator::Associativty::Right;
      break;
    case ':':
      type         = CJOperator::Type::Colon;
      precedence   = 12;
      associativty = CJOperator::Associativty::Right;
      break;

    default:
      assert(false);
      break;
  }

  tokens_.push_back(CJTokenP(new CJOperator(type, precedence, associativty, ary)));
}

void
CJavaScript::
readDoubleString(CStrParse &parse)
{
  std::string str;

  parse.skipChar();

  while (! parse.eof() && ! parse.isChar('"')) {
    if (parse.isChar('\\')) {
      char c = parse.readChar();

      if (! parse.eof()) {
        c = parse.readChar();

        switch (c) {
          default : str += '\\'; str += c; break;
          case 'b': str += '\b'; break;
          case 't': str += '\t'; break;
          case 'n': str += '\n'; break;
          case 'v': str += '\v'; break;
          case 'f': str += '\f'; break;
          case 'r': str += '\r'; break;
          case '\"': str += '\"'; break;
          case '\'': str += '\''; break;
          case '\\': str += '\\'; break;
          case '0': case '1': case '2': case '3':
          case '4': case '5': case '6': case '7': {
            int octal = c - '0';

            if (! parse.eof() && parse.isODigit()) {
              octal = octal*8 + (parse.readChar() - '0');

              if (! parse.eof() && parse.isODigit())
                octal = octal*8 + (parse.readChar() - '0');
            }

            str += char(octal);

            break;
          }
          case 'u': {
            // TODO - 4 hexadecimal digits
            break;
          }
        }
      }
      else
        str += c;
    }
    else if (parse.isChar('"'))
      break;
    else
      str += parse.readChar();
  }

  if (parse.isChar('"'))
    parse.skipChar();

  tokens_.push_back(CJTokenP(new CJString(this, str, '\"')));
}

void
CJavaScript::
readSingleString(CStrParse &parse)
{
  std::string str;

  parse.skipChar();

  while (! parse.eof() && ! parse.isChar('\''))
    str += parse.readChar();

  if (parse.isChar('\''))
    parse.skipChar();

  tokens_.push_back(CJTokenP(new CJString(this, str, '\'')));
}

CJToken::Type
CJavaScript::
lastTokenType() const
{
  if (tokens_.empty())
    return CJToken::Type::None;
  else
    return tokens_.back()->type();
}

void
CJavaScript::
errorMsg(const std::string &msg) const
{
  std::cerr << msg << std::endl;
}

//------

std::string
CJKeyword::
name() const
{
  switch (type_) {
    case CJKeyword::Type::None    : return "<none>";
    case CJKeyword::Type::Break   : return "break";
    case CJKeyword::Type::Case    : return "case";
    case CJKeyword::Type::Catch   : return "catch";
    case CJKeyword::Type::Class   : return "class";
    case CJKeyword::Type::Const   : return "const";
    case CJKeyword::Type::Continue: return "continue";
    case CJKeyword::Type::Default : return "default";
    case CJKeyword::Type::Delete  : return "delete";
    case CJKeyword::Type::Do      : return "do";
    case CJKeyword::Type::Else    : return "else";
    case CJKeyword::Type::Finally : return "finally";
    case CJKeyword::Type::For     : return "for";
    case CJKeyword::Type::Function: return "function";
    case CJKeyword::Type::If      : return "if";
    case CJKeyword::Type::In      : return "in";
    case CJKeyword::Type::New     : return "new";
    case CJKeyword::Type::Return  : return "return";
    case CJKeyword::Type::Switch  : return "switch";
    case CJKeyword::Type::This    : return "this";
    case CJKeyword::Type::Throw   : return "throw";
    case CJKeyword::Type::Try     : return "try";
    case CJKeyword::Type::Var     : return "var";
    case CJKeyword::Type::Void    : return "void";
    case CJKeyword::Type::While   : return "while";
    case CJKeyword::Type::With    : return "with";
  }

  assert(false);

  return "";
}

//------

std::string
CJOperator::
name() const
{
  switch (type_) {
    case Type::None                    : return "<none>";
    case Type::UnaryPlus               : return "+";
    case Type::Plus                    : return "+";
    case Type::PlusAssign              : return "+=";
    case Type::UnaryMinus              : return "-";
    case Type::Minus                   : return "-";
    case Type::MinusAssign             : return "-=";
    case Type::Times                   : return "*";
    case Type::TimesAssign             : return "*=";
    case Type::Divide                  : return "/";
    case Type::DivideAssign            : return "/=";
    case Type::Modulus                 : return "%";
    case Type::ModulusAssign           : return "%=";
    case Type::Increment               : return "++";
    case Type::Decrement               : return "--";
    case Type::OpenRBracket            : return "(";
    case Type::CloseRBracket           : return ")";
    case Type::OpenSBracket            : return "[";
    case Type::CloseSBracket           : return "]";
    case Type::OpenBrace               : return "{";
    case Type::CloseBrace              : return "}";
    case Type::Less                    : return "<";
    case Type::LessEqual               : return "<=";
    case Type::Greater                 : return ">";
    case Type::GreaterEqual            : return ">=";
    case Type::Equal                   : return "==";
    case Type::StrictEqual             : return "===";
    case Type::NotEqual                : return "!=";
    case Type::StrictNotEqual          : return "!==";
    case Type::Assign                  : return "=";
    case Type::LeftShift               : return "<<";
    case Type::LeftShiftAssign         : return "<<=";
    case Type::RightShift              : return ">>";
    case Type::RightShiftAssign        : return ">>=";
    case Type::UnsignedRightShift      : return ">>>";
    case Type::UnsignedRightShiftAssign: return ">>>=";
    case Type::LogicalAnd              : return "&&";
    case Type::LogicalOr               : return "||";
    case Type::LogicalNot              : return "!";
    case Type::BitwiseAnd              : return "&";
    case Type::BitwiseAndAssign        : return "&=";
    case Type::BitwiseOr               : return "|";
    case Type::BitwiseOrAssign         : return "|=";
    case Type::BitwiseNot              : return "~";
    case Type::BitwiseNotAssign        : return "~=";
    case Type::BitwiseXor              : return "^";
    case Type::BitwiseXorAssign        : return "^=";
    case Type::Scope                   : return ".";
    case Type::Comma                   : return ",";
    case Type::SemiColon               : return ";";
    case Type::Question                : return "?";
    case Type::Colon                   : return ":";
    case Type::TypeOf                  : return "typeof";
    default                            : assert(false);
  }

  return "";
}

//------

CJScope::
CJScope(CJavaScript *js, const std::string &name) :
 CJDictionary(js), name_(name)
{
}

void
CJScope::
addScope(CJScopeP scope)
{
  scopes_[scope->name()] = scope;
}

void
CJScope::
removeScope(const std::string &name)
{
  scopes_.erase(name);
}

std::vector<std::string>
CJScope::
getFunctionNames() const
{
  std::vector<std::string> names;

  for (const auto &f : keyValues_) {
    if (f.second->type() == CJToken::Type::Function)
      names.push_back(f.first);
  }

  return names;
}

std::vector<std::string>
CJScope::
getVariableNames() const
{
  std::vector<std::string> names;

  for (const auto &v : keyValues_) {
    if (v.second->type() == CJToken::Type::Var)
      names.push_back(v.first);
  }

  return names;
}

CJScopeP
CJScope::
lookupScope(const std::string &name)
{
  auto p = scopes_.find(name);

  if (p == scopes_.end())
    return CJScopeP();

  return (*p).second;
}

//------

CJValueP
CJExecAssignExpression::
exec(CJavaScript *js)
{
  CJValueP value;

  if (! lexpr_ || ! rexpr_)
    return value;

  const CJExecExpression::Tokens &tokens = lexpr_->tokens();

  CJTokenP varToken;

  if (tokens.size() == 1)
    varToken = tokens[0];

  CJExecIdentifiers *identifiers = 0;
  CJExecExpression  *indexExpr = 0;

  if      (varToken && varToken->type() == CJToken::Type::IndexExpression) {
    identifiers = varToken->cast<CJExecIndexExpression>()->identifiers().get();
    indexExpr   = varToken->cast<CJExecIndexExpression>()->indexExpression().get();
  }
  else if (varToken && varToken->type() == CJToken::Type::Identifiers) {
    identifiers = varToken->cast<CJExecIdentifiers>();
  }

  if (! identifiers) {
    js->errorMsg("Missing variable name for assign");
    return value;
  }

  CJLValueP lvalue = js->lookupLValue(identifiers->identifiers());

  if (! lvalue)
    lvalue = js->lookupProperty(identifiers->identifiers(), /*create*/true);

  CJValueP rvalue = rexpr_->exec(js);

  if (! rvalue)
    return value;

  CJValueP varValue;

  if (lvalue)
    varValue = lvalue->value();

  if (! varValue)
    varValue = js->createNumberValue(long(0));

  CJOperator::Type opType = op_->type();

  switch (opType) {
    case CJOperator::Type::Assign:
      break;
    case CJOperator::Type::PlusAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Plus, varValue, rvalue);
      break;
    case CJOperator::Type::MinusAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Minus, varValue, rvalue);
      break;
    case CJOperator::Type::TimesAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Times, varValue, rvalue);
      break;
    case CJOperator::Type::DivideAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Divide, varValue, rvalue);
      break;
    case CJOperator::Type::ModulusAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Modulus, varValue, rvalue);
      break;
    case CJOperator::Type::LeftShiftAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::LeftShift, varValue, rvalue);
      break;
    case CJOperator::Type::RightShiftAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::RightShift, varValue, rvalue);
      break;
    case CJOperator::Type::UnsignedRightShiftAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::UnsignedRightShift, varValue, rvalue);
      break;
    case CJOperator::Type::BitwiseAndAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseAnd, varValue, rvalue);
      break;
    case CJOperator::Type::BitwiseOrAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseOr, varValue, rvalue);
      break;
    case CJOperator::Type::BitwiseNotAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseNot, varValue, rvalue);
      break;
    case CJOperator::Type::BitwiseXorAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseXor, varValue, rvalue);
      break;
    default:
      assert(false);
      break;
  }

  if (indexExpr) {
    CJValueP ivalue = indexExpr->exec(js);

    long ind = ivalue->toInteger();

    if (varValue->hasIndex()) {
      varValue->setIndexValue(ind, rvalue);
    }
    else {
      js->errorMsg("Variable is not an array");
      return value;
    }
  }
  else {
    if (lvalue)
      lvalue->setValue(rvalue);
  }

  return rvalue;
}

//------

CJValueP
CJExecIndexExpression::
exec(CJavaScript *js)
{
  CJValueP value;

  if ((! identifiers_ && ! expr_) || ! iexpr_)
    return value;

  CJValueP avalue;

  if (identifiers_) {
    CJLValueP varValue = js->lookupProperty(identifiers_->identifiers());

    if (varValue)
      avalue = varValue->value();
  }
  else {
    avalue = expr_->exec(js);
  }

  if (! avalue) {
    js->errorMsg("Invalid index value");
    return value;
  }

  CJValueP res;

  if       (avalue->type() == CJToken::Type::Array) {
    CJArray *array = avalue->cast<CJArray>();

    CJValueP ivalue = iexpr_->exec(js);

    long ind = ivalue->toInteger();

    res = array->indexValue(ind);
  }
  else if (avalue->type() == CJToken::Type::Dictionary) {
    CJDictionary *dict = avalue->cast<CJDictionary>();

    CJValueP ivalue = iexpr_->exec(js);

    res = dict->lookupProperty(ivalue->toString());
  }
  else {
    CJValueP ivalue = iexpr_->exec(js);

    long ind = ivalue->toInteger();

    if (! avalue->hasIndex()) {
      js->errorMsg("Value not an array for index");
      return res;
    }

    res = avalue->indexValue(ind);
  }

  if (! res)
    res = CJValueP(new CJUndefined(js));

  return res;
}

//------

CJValueP
CJExecIncrDecrExpression::
exec(CJavaScript *js)
{
  CJValueP value;

  if (! identifiers_ || ! op_)
    return value;

  CJLValueP varValue = js->lookupProperty(identifiers_->identifiers(), /*create*/true);

  CJOperator::Type opType = op_->type();

  if (varValue)
    value = varValue->value();

  CJValueP value1 = js->execUnaryOp(opType, value);

  varValue->setValue(value1);

  return (post_ ? value : value1);
}

//------

void
CJExecBlock::
interp(CJavaScript *js)
{
  CJavaScript::ExecData execData;

  execData.setBlock(true);

  execData.initExec(tokens_);

  js->interp(execData);

  etokens_ = execData.etokens();
}

CJValueP
CJExecBlock::
exec(CJavaScript *js)
{
  CJValueP value;

  for (auto &etoken : etokens_) {
    if (etoken->isValue()) {
      CJValueP value1(etoken.get()->cast<CJValue>()->dup(js));

      value = value1;
    }
    else
      value = etoken->exec(js);

    if (isBreakFlag() || isContinueFlag())
      break;
  }

  return value;
}

//------

CJValueP
CJExecExpression::
exec(CJavaScript *js)
{
  typedef std::vector<CJValueP>     Values;
  typedef std::vector<CJOperator *> Operators;

  Values    values;
  Operators operators;

  CJOperator *lastOp = 0;

  int len = tokens_.size();

  for (int i = 0; i < len; ++i) {
    CJTokenP token = tokens_[i];

    if (! token)
      return CJValueP();

    CJToken::Type type = token->type();

    if      (token->isValue()) {
      CJValueP value(token.get()->cast<CJValue>()->dup(js));

      values.push_back(value);
    }
    else if (type == CJToken::Type::Operator) {
      CJOperator *op = token->cast<CJOperator>();

      bool unstack = false;

      if (lastOp) {
        if      (lastOp->precedence() < op->precedence())
          unstack = true;
        else if (lastOp->precedence() == op->precedence())
          unstack = (lastOp->associativty() == CJOperator::Associativty::Left);
      }

      if (unstack) {
        operators.pop_back(); // lastOp

        CJValueP value;

        if (lastOp->ary() == CJOperator::Ary::Binary) {
          if (values.size() < 2) {
            js->errorMsg("Missing values for binary op");
            return CJValueP();
          }

          CJValueP value2 = values.back(); values.pop_back();
          CJValueP value1 = values.back(); values.pop_back();

          value = js->execBinaryOp(lastOp->type(), value1, value2);
        }
        else {
          if (values.size() < 1) {
            js->errorMsg("Missing value for unary op");
            return CJValueP();
          }

          CJValueP value1 = values.back(); values.pop_back();

          value = js->execUnaryOp(lastOp->type(), value1);
        }

        if (value)
          values.push_back(value);
        else
          js->errorMsg("Invalid operator value");
      }

      operators.push_back(op);

      lastOp = op;
    }
    else if (type == CJToken::Type::Identifiers) {
      CJExecIdentifiers *identifiers = token->cast<CJExecIdentifiers>();

      CJValueP value = js->lookupValue(identifiers->identifiers());

      if (! value) {
        std::stringstream ss; ss << *identifiers;
        js->errorMsg("Invalid named value : " + ss.str());
      }

      values.push_back(value);
    }
    else {
      bool shortCircuit = false;

      if (values.size() == 1 && operators.size() == 1 &&
          (lastOp->type() == CJOperator::Type::LogicalAnd ||
           lastOp->type() == CJOperator::Type::LogicalOr)) {
        bool b = values[0]->toBoolean();

        if      (! b && lastOp->type() == CJOperator::Type::LogicalAnd)
          shortCircuit = true;
        else if (  b && lastOp->type() == CJOperator::Type::LogicalOr)
          shortCircuit = true;

        if (shortCircuit) {
          return CJValueP(js->createBoolValue(b));
        }
      }

      CJValueP value = token->exec(js);

      values.push_back(value);
    }
  }

  while (! operators.empty()) {
    lastOp = operators.back(); operators.pop_back();

    CJValueP value;

    if (lastOp->ary() == CJOperator::Ary::Binary) {
      if (values.size() < 2) {
        js->errorMsg("Missing values for binary op");
        return CJValueP();
      }

      CJValueP value2 = values.back(); values.pop_back();
      CJValueP value1 = values.back(); values.pop_back();

      value = js->execBinaryOp(lastOp->type(), value1, value2);
    }
    else {
      if (values.size() < 1) {
        js->errorMsg("Missing value for unary op");
        return CJValueP();
      }

      CJValueP value1 = values.back(); values.pop_back();

      value = js->execUnaryOp(lastOp->type(), value1);
    }

    if (value)
      values.push_back(value);
    else
      js->errorMsg("Invalid operator value");
  }

  if (values.empty())
    return CJValueP();

  return values.back();
}

//------

CJValueP
CJExecFunction::
exec(CJavaScript *js)
{
  if (! identifiers_) {
    js->errorMsg("Missing function name");
    return CJValueP();
  }

  typedef std::vector<CJValueP> Values;

  Values values;

  CJValueP fn;

  if (expr_) {
    CJValueP evalue = expr_->exec(js);

    if (identifiers_->identifiers().size() != 1) {
      std::stringstream ss; ss << *identifiers_;
      js->errorMsg("Invalid type function identifiers : " + ss.str());
      return CJValueP();
    }

    fn = js->getTypeFunction(evalue->valueType(), identifiers_->identifiers()[0]->name());

    if (! fn) {
      std::stringstream ss; ss << *identifiers_;
      js->errorMsg("Function not found : " + ss.str());
      return CJValueP();
    }

    values.push_back(evalue);
  }
  else {
    CJLValueP varValue = js->lookupProperty(identifiers_->identifiers());

    if (! varValue) {
      auto fnValue = js->variableFunction(identifiers_->identifiers());

      if (fnValue.first) {
        fn = fnValue.first;

        values.push_back(fnValue.second);
      }
    }
    else {
      fn = varValue->value();
    }

    if (! fn || fn->type() != CJToken::Type::Function) {
      std::stringstream ss; ss << *identifiers_;
      js->errorMsg("Function not found : " + ss.str());
      return CJValueP();
    }
  }

  CJExecExpressionList::Values values1;

  if (exprList_)
    values1 = exprList_->getValues(js);

  for (auto &v1 : values1)
    values.push_back(v1);

  if (! fn) {
    std::stringstream ss; ss << *identifiers_;
    js->errorMsg("Function not found : " + ss.str());
    return CJValueP();
  }

  return fn->cast<CJFunction>()->exec(js, values);
}

//------

CJValueP
CJExecFor::
exec(CJavaScript *js)
{
  // for in
  if      (inExpr_) {
    if (! identifiers_) {
      js->errorMsg("Missing variable name for for in");
      return CJValueP();
    }

    CJLValueP ivalue = js->lookupLValue(identifiers_->identifiers());

    if (! ivalue)
      ivalue = js->lookupProperty(identifiers_->identifiers(), /*create*/true);

    //---

    CJValueP value = inExpr_->exec(js);

    if (! value->hasIndex()) {
      js->errorMsg("Non-index value : for in ");
      return CJValueP();
    }

    int len = value->length();

    for (int i = 0; i < len; ++i) {
      CJValueP value1 = value->indexValue(i);

      ivalue->setValue(value1);

      if (block_) {
        js->startBlock(block_);

        block_->exec(js);

        bool breakFlag = block_->isBreakFlag();

        js->endBlock();

        if (breakFlag)
          break;
      }
    }
  }
  // for (<expr1>; <expr2>; <expr3>)
  // for (var <expr1>; <expr2>; <expr3>)
  else if (exprList2_ && exprList3_) {
    if      (exprList1_)
      (void) exprList1_->exec(js);
    else if (var_)
      (void) var_->exec(js);
    else
      return CJValueP();

    //---

    CJValueP value2 = exprList2_->exec(js);

    while (value2 && value2->toBoolean()) {
      if (block_) {
        js->startBlock(block_);

        block_->exec(js);

        bool breakFlag = block_->isBreakFlag();

        js->endBlock();

        if (breakFlag)
          break;
      }

      (void) exprList3_->exec(js);

      value2 = exprList2_->exec(js);
    }
  }

  return CJValueP();
}

//------

CJValueP
CJExecIf::
exec(CJavaScript *js)
{
  if (! exprList_ || ! ifBlock_)
    return CJValueP();

  CJValueP value = exprList_->exec(js);

  CJValueP value1;

  if (value) {
    if (value->toBoolean()) {
      js->startBlock(ifBlock_);

      value1 = ifBlock_->exec(js);

      js->endBlock();
    }
    else {
      if (elseBlock_) {
        js->startBlock(elseBlock_);

        value1 = elseBlock_->exec(js);

        js->endBlock();
      }
    }
  }

  return value1;
}

//------

CJValueP
CJExecWhile::
exec(CJavaScript *js)
{
  if (! exprList_ || ! block_)
    return CJValueP();

  CJValueP value = exprList_->exec(js);

  CJValueP value1;

  while (value && value->toBoolean()) {
    js->startBlock(block_);

    value1 = block_->exec(js);

    js->endBlock();

    if (block_->isBreakFlag())
      break;

    value = exprList_->exec(js);
  }

  return value1;
}

//------

CJValueP
CJExecSwitch::
exec(CJavaScript *js)
{
  if (! exprList_)
    return CJValueP();

  CJValueP value = exprList_->exec(js);

  CJValueP value1;

  bool found = false;

  for (const auto &c : caseBlocks_) {
    if (c.expr) {
      CJValueP v = c.expr->exec(js);

      if (js->cmp(v, value) == 0) {
        if (c.block) {
          js->startBlock(c.block);

          c.block->exec(js);

          js->endBlock();
        }

        found = true;

        break;
      }
    }
  }

  if (! found) {
    if (defaultBlock_.block) {
      js->startBlock(defaultBlock_.block);

      defaultBlock_.block->exec(js);

      js->endBlock();
    }
  }

  return CJValueP();
}

//------

CJValueP
CJExecTry::
exec(CJavaScript *js)
{
  if (! tryBlock_)
    return CJValueP();

  js->startBlock(tryBlock_);

  CJValueP value = tryBlock_->exec(js);

  js->endBlock();

  return value;
}

//------

CJValueP
CJExecQuestion::
exec(CJavaScript *js)
{
  if (! bexpr_ || ! expr1_ || ! expr2_)
    return CJValueP();

  CJValueP value = bexpr_->exec(js);

  CJValueP value1;

  if (value) {
    if (value->toBoolean())
      value1 = expr1_->exec(js);
    else
      value1 = expr2_->exec(js);
  }

  return value1;
}

//------

CJValueP
CJExecVar::
exec(CJavaScript *js)
{
  CJLValueP varValue = js->lookupProperty(identifiers_->identifiers());

  if (! varValue)
    varValue = js->lookupProperty(identifiers_->identifiers(), /*create*/true);

  CJValueP value;

  if      (exprList_)
    value = exprList_->exec(js);
  else if (block_)
    value = block_->exec(js);
  else
    value = value_;

  if (varValue) {
    if (! value)
      value = CJValueP(new CJUndefined(js));

    varValue->setValue(value);
  }

  return value;
}

//------

CJValueP
CJExecBreak::
exec(CJavaScript *js)
{
  CJExecBlockP block = js->getCurrentBlock();

  if (block)
    block->setBreakFlag(true);

  return CJValueP();
}

CJValueP
CJExecContinue::
exec(CJavaScript *js)
{
  CJExecBlockP block = js->getCurrentBlock();

  if (block)
    block->setContinueFlag(true);

  return CJValueP();
}

CJValueP
CJExecReturn::
exec(CJavaScript *js)
{
  CJValueP value;

  if (expr_)
    value = expr_->exec(js);

  CJExecBlockP block = js->getCurrentBlock();

  if (block && value)
    block->setRetVal(value);

  return value;
}

//------

CJValueP
CJExecDelete::
exec(CJavaScript *js)
{
  CJValueP value;

  js->deleteProperty(identifiers_->identifiers());

  return value;
}

//------

CJValueP
CJRealFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 1) {
    js->errorMsg("Wrong number of function values");
    return CJValueP();
  }

  if (! values[0])
    return CJValueP();

  double r = values[0]->toReal();

  double res = (*fn_)(r);

  return js->createNumberValue(res);
}

//------

CJValueP
CJReal2Function::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 2) {
    js->errorMsg("Wrong number of function values");
    return CJValueP();
  }

  double r1 = values[0]->toReal();
  double r2 = values[1]->toReal();

  double res = (*fn_)(r1, r2);

  return js->createNumberValue(res);
}

//------

CJValueP
CJUserFunction::
exec(CJavaScript *js, const Values &values)
{
  CJScopeP scope = CJScopeP(new CJScope(js, name_));

  if (values.size() != args_.size()) {
    js->errorMsg("Wrong number of function values");
    return CJValueP();
  }

  int nv = values.size();

  for (int i = 0; i < nv; ++i)
    scope->setProperty(args_[i], values[i]);

  js->startScope(scope);

  js->startBlock(block_);

  block_->exec(js);

  js->endBlock();

  js->endScope();

  return block_->getRetVal();
}

//------

CJValueP
CJObjectFunction::
exec(CJavaScript *js, const Values &values)
{
  assert(values.size() > 0);

  CJValueP value;

  if (! values[0])
    return value;

  CJObject *obj = values[0].get()->cast<CJObject>();

  if (name_ == "getOwnPropertyNames") {
    CJArray *array = new CJArray(js);

    CJObjectTypeP valueType = obj->valueType();

    if (valueType->type() == CJToken::Type::Dictionary) {
      CJDictionary *dict = obj->cast<CJDictionary>();

      for (auto &kv : dict->keyValues()) {
        std::string key = kv.first;

        array->addValue(js->createStringValue(key));
      }
    }
    else {
      auto functions = valueType->functions();

      for (const auto &f : functions)
        array->addValue(js->createStringValue(f));
    }

    value = CJValueP(array);
  }
  else
    value = obj->execNameFn(js, name(), values);

  return value;
}

//------

CJValueP
CJAlertFunction::
exec(CJavaScript *, const Values &values)
{
  int i = 0;

  for (auto &v : values) {
    if (i > 0) std::cout << " ";

    std::cerr << *v;

    ++i;
  }

  std::cerr << std::endl;

  return CJValueP();
}

CJValueP
CJSetInterval::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 2)
    return CJValueP();

  // create timer to run proc every n miliseconds
  std::string proc     = values[0]->toString();
  double      interval = values[1]->toReal();

  long timer = js->setInterval(proc, interval);

  return js->createNumberValue(long(timer));
}

CJValueP
CJClearInterval::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 1)
    return CJValueP();

  long timer = values[0]->toInteger();

  js->clearInterval(timer);

  return CJValueP();
}

//------

CJValueP
CJDocumentType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJDocumentObject::
CJDocumentObject(CJavaScript *js) :
 CJObject(js->documentType()), js_(js)
{
  type_->addFunction("write");
}

CJValueP
CJDocumentObject::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if (name == "write") {
    int n = values.size();

    for (int i = 1; i < n; ++i) {
      if (i > 1) std::cout << " ";

      std::cout << *values[i];
    }

    std::cout << std::endl;
  }
  else
    js->errorMsg("Invalid document function - " + name);

  return CJValueP();
}

//------

CJValueP
CJConsoleType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJConsoleObject::
CJConsoleObject(CJavaScript *js) :
 CJObject(js->consoleType()), js_(js)
{
  type_->addFunction("log");
}

CJValueP
CJConsoleObject::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if (name == "log") {
    int n = values.size();

    for (int i = 1; i < n; ++i) {
      if (i > 1) std::cout << " ";

      if (values[i]) {
        if (values[i]->type() == CJToken::Type::String)
          std::cout << values[i]->cast<CJString>()->text();
        else
          std::cout << *values[i];
      }
    }

    std::cout << std::endl;
  }
  else
    js->errorMsg("Invalid console function - " + name);

  return CJValueP();
}

//------

CJValueP
CJUndefinedType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJValueP
CJNullType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJValueP
CJTrueType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJValueP
CJFalseType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJValueP
CJNumberType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJValueP
CJDictionaryType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJValueP
CJFunctionType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJValueP
CJArrayType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJArray *array = values[0]->cast<CJArray>();
  assert(array);

  if      (name == "concat") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    if (values[1]->hasIndex()) {
      int len = values[1]->length();

      for (int i = 0; i < len; ++i) {
        array->addValue(values[1]->indexValue(i));
      }
    }
    else {
      array->addValue(values[1]);
    }

    return values[0];
  }
  else if (name == "join") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string jstr = values[1]->toString();

    std::string str;

    int len = array->length();

    for (int i = 0; i < len; ++i) {
      if (i > 0)
        str += jstr;

      str += array->indexValue(i)->toString();
    }

    return js->createStringValue(str);
  }
  else if (name == "length") {
    long len = array->length();

    return js->createNumberValue(len);
  }
  else if (name == "pop") {
    if (values.size() != 1) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    int len = array->length();

    if (len < 1) {
      js->errorMsg("Empty array for " + name);
      return CJValueP();
    }

    CJValueP value = array->removeValue();

    return value;
  }
  else if (name == "push") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    array->addValue(values[1]);

    return values[0];
  }
  else if (name == "shift") {
    if (values.size() != 1) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    int len = array->length();

    if (len < 1) {
      js->errorMsg("Empty array for " + name);
      return CJValueP();
    }

    CJValueP value = array->removeFrontValue();

    return value;
  }
  else if (name == "unshift") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    array->addFrontValue(values[1]);

    return values[0];
  }
  else {
    js->errorMsg("Invalid array function " + name);
  }

  return CJValueP();
}

//------

CJValueP
CJStringType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJString *cstr = values[0]->cast<CJString>();
  assert(cstr);

  std::string str = cstr->text();

  if      (name == "charAt") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    long ind = (values[1] ? values[1]->toInteger() : 0);

    if (ind >= 0 && ind < long(str.size())) {
      return js->createStringValue(str.substr(ind, 1));
    }
  }
  else if (name == "concat") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string str1 = (values[1] ? values[1]->toString() : std::string());

    return js->createStringValue(str + str1);
  }
  else if (name == "indexOf") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    CJString *key = (values[1] ? values[1]->cast<CJString>() : 0);

    if (key) {
      auto p = str.find(key->text());

      if (p == std::string::npos)
        return js->createNumberValue(long(-1));

      return js->createNumberValue(long(p));
    }
  }
  else if (name == "lastIndexOf") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    CJString *key = (values[1] ? values[1]->cast<CJString>() : 0);

    if (key) {
      auto p = str.rfind(key->text());

      if (p == std::string::npos)
        return js->createNumberValue(long(-1));

      return js->createNumberValue(long(p));
    }
  }
  else if (name == "length") {
    long len = cstr->length();

    return js->createNumberValue(len);
  }
  else if (name == "replace") {
    if (values.size() != 3) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::string str1 = (values[1] ? values[1]->toString() : std::string());
    std::string str2 = (values[2] ? values[2]->toString() : std::string());

    auto pos = str.find(str1);

    if (pos != std::string::npos)
      str.replace(pos, str1.length(), str2);

    return js->createStringValue(str);
  }
  else if (name == "slice") {
    if (values.size() != 2 && values.size() != 3) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    long ind1 = (values[1] ? values[1]->toInteger() : 0);

    if (values.size() == 3) {
      long ind2 = (values[2] ? values[2]->toInteger() : 0);

      if ((ind1 >= 0 && ind1 < long(str.size())) &&
          (ind2 >= 0 && ind2 < long(str.size())) &&
          ind1 <= ind2) {
        return js->createStringValue(str.substr(ind1, ind2 - ind1));
      }
    }
    else {
      if (ind1 >= 0 && ind1 < long(str.size())) {
        return js->createStringValue(str.substr(ind1));
      }
    }
  }
  else if (name == "split") {
    if (values.size() != 2) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    std::vector<std::string> strs;

    std::string sstr = (values[1] ? values[1]->toString() : std::string());

    auto p = str.find(sstr);

    while (p != std::string::npos) {
      strs.push_back(str.substr(0, p));

      str = str.substr(p + sstr.size());

      p = str.find(sstr);
    }

    strs.push_back(str);

    CJArray *array = new CJArray(js);

    for (const auto &s : strs) {
      CJValueP value = js->createStringValue(s);

      array->addValue(value);
    }

    return CJValueP(array);
  }
  else if (name == "substr") {
    if (values.size() != 2 && values.size() != 3) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    long start = (values[1] ? values[1]->toInteger() : 0);

    if (values.size() == 3) {
      long len = (values[2] ? values[2]->toInteger() : 0);

      long end = start + len;

      if ((start >= 0 && start < long(str.size())) &&
          (end   >= 0 && end   < long(str.size())) &&
          start <= end) {
        return js->createStringValue(str.substr(start, end - start));
      }
    }
    else {
      if (start >= 0 && start < long(str.size())) {
        return js->createStringValue(str.substr(start));
      }
    }
  }
  else if (name == "substring") {
    if (values.size() != 2 && values.size() != 3) {
      js->errorMsg("Invalid number of arguments for " + name);
      return CJValueP();
    }

    long ind1 = (values[1] ? values[1]->toInteger() : 0);

    if (values.size() == 3) {
      long ind2 = (values[2] ? values[2]->toInteger() : 0);

      if ((ind1 >= 0 && ind1 < long(str.size())) &&
          (ind2 >= 0 && ind2 < long(str.size())) &&
          ind1 <= ind2) {
        return js->createStringValue(str.substr(ind1, ind2 - ind1));
      }
    }
    else {
      if (ind1 >= 0 && ind1 < long(str.size())) {
        return js->createStringValue(str.substr(ind1));
      }
    }
  }
  else if (name == "toLowerCase") {
    for (size_t i = 0; i < str.size(); ++i)
      str[i] = tolower(str[i]);

    return js->createStringValue(str);
  }
  else if (name == "toUpperCase") {
    for (size_t i = 0; i < str.size(); ++i)
      str[i] = toupper(str[i]);

    return js->createStringValue(str);
  }
  else {
    js->errorMsg("Invalid string function " + name);
  }

  return CJValueP();
}

//------

CJUndefined::
CJUndefined(CJavaScript *js) :
 CJValue(js->undefinedType())
{
}

//------

CJNull::
CJNull(CJavaScript *js) :
 CJValue(js->nullType())
{
}

//------

CJTrue::
CJTrue(CJavaScript *js) :
 CJValue(js->trueType())
{
}

//------

CJFalse::
CJFalse(CJavaScript *js) :
 CJValue(js->falseType())
{
}

//------

CJNumber::
CJNumber(CJavaScript *js, double real) :
 CJValue(js->numberType()), real_(real)
{
}

//------

CJString::
CJString(CJavaScript *js, const std::string &text, char c) :
 CJValue(js->stringType()), text_(text), c_(c)
{
}

//------

CJArray::
CJArray(CJavaScript *js, int n) :
 CJValue(js->arrayType())
{
  if (n > 0)
    values_.resize(n);
}

CJArray::
CJArray(CJavaScript *js, const Values &values) :
 CJValue(js->arrayType()), values_(values)
{
}

CJArray::
CJArray(CJavaScript *js, const std::vector<CJValueP> &values) :
 CJValue(js->arrayType())
{
  for (const auto &v : values)
    values_.push_back(v);
}

//------

CJDictionary::
CJDictionary(CJavaScript *js, const KeyValues &keyValues) :
 CJValue(js->dictType()), keyValues_(keyValues)
{
}

//------

CJFunction::
CJFunction(CJavaScript *js, const std::string &name, Type type) :
 CJValue(js->funcType()), name_(name), type_(type)
{
}

//------

CJScopeValue::
CJScopeValue(CJavaScript *js, CJScope *scope, const std::string &name) :
 CJLValue(js->dictType()), scope_(scope), name_(name)
{
}

//------

CJDictValue::
CJDictValue(CJavaScript *js, CJDictionary *dict, const std::string &name) :
 CJLValue(js->dictType()), js_(js), dict_(dict), name_(name)
{
}

//------

CJDictionaryRef::
CJDictionaryRef(CJavaScript *js, CJDictionary *dict, const std::string &name) :
 CJLValue(js->dictType()), js_(js), dict_(dict), name_(name)
{
}

CJValueP
CJDictionaryRef::
value() const
{
  return dict_->lookupProperty(name_);
}

void
CJDictionaryRef::
setValue(CJValueP value)
{
  dict_->setIndexValue(js_, name_, value);
}

//------

CJObjectValue::
CJObjectValue(CJavaScript *js, CJObjectP obj, const std::string &name) :
 CJLValue(js->dictType()), js_(js), obj_(obj), name_(name)
{
}

CJValueP
CJObjectValue::
value() const
{
  return obj_->getNameValue(js_, name_);
}

void
CJObjectValue::
setValue(CJValueP value)
{
  obj_->setNameValue(js_, name_, value);
}

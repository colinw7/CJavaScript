#include <CJavaScript.h>
#include <CFile.h>
#include <CStrUtil.h>
#include <CStrParse.h>
#include <COSNaN.h>
#include <iostream>
#include <cassert>
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
  scope_ = CJScopeP(new CJScope(""));

  // Global
  scope_->addFunction(CJFunctionP(new CJAlertFunction));
  scope_->addFunction(CJFunctionP(new CJSetInterval  ));
  scope_->addFunction(CJFunctionP(new CJClearInterval));

  // Math
  CJScopeP mathScope = createScope("Math");

  mathScope->addVariable(CJVariableP(new CJVariable("PI", M_PI)));

  mathScope->addFunction(CJFunctionP(new CJRealFunction("sin", sin)));
  mathScope->addFunction(CJFunctionP(new CJRealFunction("cos", cos)));
  mathScope->addFunction(CJFunctionP(new CJRealFunction("tan", tan)));

  // document
  CJScopeP docScope = createScope("document");

  docScope->addFunction(CJFunctionP(new CJDocumentWriteFunction));

  // String
  addTypeFunction(CJToken::Type::String, CJFunctionP(new CJStringCharAtFunction   ));
  addTypeFunction(CJToken::Type::String, CJFunctionP(new CJStringIndexOfFunction  ));
  addTypeFunction(CJToken::Type::String, CJFunctionP(new CJStringLengthFunction   ));
  addTypeFunction(CJToken::Type::String, CJFunctionP(new CJStringSubStringFunction));
}

CJScopeP
CJavaScript::
createScope(const std::string &name)
{
  CJScopeP scope = CJScopeP(new CJScope(name));

  scope_->addScope(scope);

  return scope;
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
addFunction(CJFunctionP fn)
{
  scope_->addFunction(fn);
}

void
CJavaScript::
addVariable(CJVariableP var)
{
  scope_->addVariable(var);
}

CJFunctionP
CJavaScript::
lookupFunction(const Identifiers &identifiers)
{
  CJScope *scope = currentScope().get();

  CJFunctionP fn = lookupFunction(scope, identifiers);

  while (! fn && scope->getParent()) {
    scope = scope->getParent();

    fn = lookupFunction(scope, identifiers);
  }

  return fn;
}

CJFunctionP
CJavaScript::
lookupFunction(CJScope *scope, const Identifiers &identifiers)
{
  int len = identifiers.size();

  CJFunctionP fn;

  for (int i = 0; i < len; ++i) {
    if (i < len - 1) {
      CJScopeP scope1 = scope->lookupScope(identifiers[i]->name());

      if (! scope1)
        break;

      scope = scope1.get();
    }
    else
      fn = scope->lookupFunction(identifiers[i]->name());
  }

  return fn;
}

CJVariableP
CJavaScript::
lookupVariable(const Identifiers &identifiers, bool create)
{
  CJScope *scope = currentScope().get();

  CJVariableP var = lookupVariable(scope, identifiers, /*create*/false);

  while (! var && scope->getParent()) {
    scope = scope->getParent();

    var = lookupVariable(scope, identifiers, /*create*/false);
  }

  if (! var && create) {
    scope = currentScope().get();

    var = lookupVariable(scope, identifiers, /*create*/true);
  }

  return var;
}

CJVariableP
CJavaScript::
lookupVariable(CJScope *scope, const Identifiers &identifiers, bool create)
{
  int len = identifiers.size();

  CJVariableP var;

  for (int i = 0; i < len; ++i) {
    if (i < len - 1) {
      CJScopeP scope1 = scope->lookupScope(identifiers[i]->name());

      if (! scope)
        break;

      scope = scope1.get();
    }
    else
      var = scope->lookupVariable(identifiers[i]->name(), create);
  }

  return var;
}

CJValueP
CJavaScript::
variableProperty(const Identifiers &identifiers)
{
  auto fnValue = variableFunction(identifiers);

  if (! fnValue.first)
    return CJValueP();

  CJFunction::Values values;

  values.push_back(fnValue.second);

  CJValueP value = fnValue.first->exec(this, values);

  return value;
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

  CJVariableP var;

  if (i < len - 1) {
    var = scope->lookupVariable(identifiers[i]->name(), /*create*/false);

    if (! var)
      return FunctionValuePair(0, 0);

    ++i;
  }

  if (! var)
    return FunctionValuePair(0, 0);

  //---

  CJValueP value = var->value(/*create*/false);

  while (value && i < len - 1) {
    CJFunctionP fn = getTypeFunction(value->type(), identifiers[i]->name());

    if (! fn)
      return FunctionValuePair(0, 0);

    CJFunction::Values values;

    values.push_back(value);

    value = fn->exec(this, values);

    ++i;
  }

  CJFunctionP fn;

  if (value && i < len)
    fn = getTypeFunction(value->type(), identifiers[i]->name());

  return FunctionValuePair(fn, value);
}

CJValueP
CJavaScript::
valueProperty(CJValueP value, const std::string &name)
{
  assert(value && name.length());

  return CJValueP();
}

CJVariableP
CJavaScript::
setVariable(const Identifiers &identifiers, CJValueP value)
{
  CJVariableP var = lookupVariable(identifiers, /*create*/true);

  if (var)
    var->setValue(value);

  return var;
}

void
CJavaScript::
addTypeFunction(CJToken::Type type, CJFunctionP fn)
{
  typeFunctions_[type][fn->name()] = fn;
}

CJFunctionP
CJavaScript::
getTypeFunction(CJToken::Type type, const std::string &name) const
{
  if (type == CJToken::Type::Object) {
    CJObjectFunction *fn = new CJObjectFunction(name);

    return CJFunctionP(fn);
  }

  auto p = typeFunctions_.find(type);

  if (p == typeFunctions_.end())
    return CJFunctionP();

  auto p1 = (*p).second.find(name);

  if (p1 == (*p).second.end())
    return CJFunctionP();

  return (*p1).second;
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

  while (! parse.eof()) {
    parse.skipSpace();

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
      readOperator(parse);
    }
    else if (parse.isChar('"')) {
      readDoubleString(parse);
    }
    else if (parse.isChar('\'')) {
      readSingleString(parse);
    }
    else {
      errorMsg("Invalid text: '" + parse.getAt() + "'");
      break;
    }
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
      std::cout << *t << " = ";

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
      CJExecExpressionList *exprList = execExpressionList();

      if (! exprList) {
        std::ostringstream ss; ss << *token;
        errorMsg("Exec failed at token: " + ss.str());
        break;
      }

      execData_->addEToken(CJTokenP(exprList));
    }
    else if (type == CJToken::Type::Operator) {
      CJExecExpressionList *exprList = execExpressionList();

      if (! exprList) {
        std::ostringstream ss; ss << *token;
        errorMsg("Exec failed at token: " + ss.str());
        break;
      }

      execData_->addEToken(CJTokenP(exprList));
    }
    else if (type == CJToken::Type::Number || type == CJToken::Type::Null ||
             type == CJToken::Type::True || type == CJToken::Type::False) {
      CJExecExpressionList *exprList = execExpressionList();

      if (! exprList) {
        std::ostringstream ss; ss << *token;
        errorMsg("Exec failed at token: " + ss.str());
        break;
      }

      execData_->addEToken(CJTokenP(exprList));
    }
    else if (type == CJToken::Type::Keyword) {
      CJKeyword *keyword = token->cast<CJKeyword>();

      if      (keyword->type() == CJKeyword::Type::For) {
        execData_->next();

        CJExecFor *efor = execFor();

        if (! efor) {
          errorMsg("Exec failed for 'for'");
          break;
        }

        execData_->addEToken(CJTokenP(efor));
      }
      else if (keyword->type() == CJKeyword::Type::If) {
        execData_->next();

        CJExecIf *eif = execIf();

        if (! eif) {
          errorMsg("Exec failed for 'if'");
          break;
        }

        execData_->addEToken(CJTokenP(eif));
      }
      else if (keyword->type() == CJKeyword::Type::While) {
        execData_->next();

        CJExecWhile *ewhile = execWhile();

        if (! ewhile) {
          errorMsg("Exec failed for 'while'");
          break;
        }

        execData_->addEToken(CJTokenP(ewhile));
      }
      else if (keyword->type() == CJKeyword::Type::Var) {
        execData_->next();

        CJExecVar *evar = execVar();

        if (! evar) {
          errorMsg("Exec failed for 'var'");
          break;
        }

        execData_->addEToken(CJTokenP(evar));
      }
      else if (keyword->type() == CJKeyword::Type::New) {
        execData_->next();

        CJValue *value = execNew();

        if (! value) {
          errorMsg("Exec failed for 'new'");
          break;
        }

        execData_->addEToken(CJTokenP(value));
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

        CJExecReturn *eret = execReturn();

        if (! eret) {
          errorMsg("Exec failed for 'return'");
          break;
        }

        execData_->addEToken(CJTokenP(eret));
      }
      else if (keyword->type() == CJKeyword::Type::Function) {
        execData_->next();

        CJFunctionP efunction = execUserFunction();

        if (! efunction) {
          errorMsg("Exec failed for 'function'");
          break;
        }
      }
      else {
        errorMsg("Exec failed at keyword: " + keyword->name());
        break;
      }
    }
    else {
      std::stringstream ss; ss << *token;
      errorMsg("Exec failed at token: " + ss.str());
      break;
    }
  }

  execData_ = execDataStack_.back();

  execDataStack_.pop_back();
}

CJExecFunction *
CJavaScript::
execFunction(CJExecIdentifiersP identifiers)
{
  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open round bracket");
    return 0;
  }

  execData_->next();

  CJExecFunction *func = new CJExecFunction(identifiers);

  CJExecExpressionList *exprList = execExpressionList();

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

  return func;
}

CJExecFor *
CJavaScript::
execFor()
{
  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open round bracket");
    return 0;
  }

  execData_->next();

  CJExecFor *efor = new CJExecFor;

  CJExecExpressionList *exprList1 = execExpressionList();

  if (! exprList1) {
    delete efor;
    return 0;
  }

  efor->setExprList1(CJExecExpressionListP(exprList1));

  CJExecExpressionList *exprList2 = execExpressionList();

  if (! exprList2) {
    delete efor;
    return 0;
  }

  efor->setExprList2(CJExecExpressionListP(exprList2));

  CJExecExpressionList *exprList3 = execExpressionList();

  if (! exprList3) {
    delete efor;
    return 0;
  }

  efor->setExprList3(CJExecExpressionListP(exprList3));

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

  CJExecBlock *block = execBlock(CJExecBlock::Type::Iterative);

  if (! block) {
    delete efor;
    return 0;
  }

  block->interp(this);

  efor->setBlock(CJExecBlockP(block));

  return efor;
}

CJExecQuestion *
CJavaScript::
execQuestion(CJExecExpression *bexpr)
{
  CJExecQuestion *equestion = new CJExecQuestion(CJExecExpressionP(bexpr));

  CJExecExpression *expr1 = execExpression();

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

  CJExecExpression *expr2 = execExpression();

  if (! expr2) {
    delete equestion;
    return 0;
  }

  equestion->setExpr2(CJExecExpressionP(expr2));

  return equestion;
}

CJExecIf *
CJavaScript::
execIf()
{
  CJExecIf *eif = new CJExecIf;

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open bracket for if");
    delete eif;
    return 0;
  }

  execData_->next();

  CJExecExpressionList *exprList = execExpressionList();

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

  CJExecBlock *ifBlock = execBlock(CJExecBlock::Type::Sequential);

  if (! ifBlock) {
    delete eif;
    return 0;
  }

  ifBlock->interp(this);

  eif->setIfBlock(CJExecBlockP(ifBlock));

  CJTokenP token = execData_->token();

  if (isKeyword(token, CJKeyword::Type::Else)) {
    execData_->next();

    CJExecBlock *elseBlock = execBlock(CJExecBlock::Type::Sequential);

    if (! elseBlock) {
      delete eif;
      return 0;
    }

    elseBlock->interp(this);

    eif->setElseBlock(CJExecBlockP(elseBlock));
  }

  return eif;
}

CJExecWhile *
CJavaScript::
execWhile()
{
  CJExecWhile *ewhile = new CJExecWhile;

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open bracket for while");
    delete ewhile;
    return 0;
  }

  execData_->next();

  CJExecExpressionList *exprList = execExpressionList();

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

  CJExecBlock *block = execBlock(CJExecBlock::Type::Iterative);

  if (! block) {
    delete ewhile;
    return 0;
  }

  block->interp(this);

  ewhile->setBlock(CJExecBlockP(block));

  return ewhile;
}

CJExecVar *
CJavaScript::
execVar()
{
  CJExecVar *var = new CJExecVar;

  while (! execData_->eof()) {
    CJTokenP token = execData_->token();

    if (token->type() != CJToken::Type::Identifier) {
      errorMsg("Missing identifier");
      delete var;
      return 0;
    }

    CJExecIdentifiers *identifiers = execIdentfiers();

    CJExecExpressionList *exprList = 0;

    token = execData_->token();

    if (isOperator(token, CJOperator::Type::Assign)) {
      execData_->next();

      exprList = execExpressionList();
    }

    var->addVar(CJExecIdentifiersP(identifiers), CJExecExpressionListP(exprList));

    if (! isOperator(token, CJOperator::Type::Comma)) {
      break;
    }

    execData_->next();
  }

  return var;
}

CJValue *
CJavaScript::
execNew()
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

  CJExecExpressionList *exprList = execExpressionList();

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg("Missing open bracket for new");
    delete exprList;
    return 0;
  }

  execData_->next();

  CJValue *value = 0;

  if      (typeName == "String") {
    CJString *str = new CJString;

    for (auto &e : exprList->expressions()) {
      CJValueP value1 = e->exec(this);

      str->setText(value1->toString());
    }

    value = str;
  }
  else if (typeName == "Array") {
    CJArray *array = new CJArray;

    for (auto &e : exprList->expressions()) {
      CJValueP value1 = e->exec(this);

      array->addValue(value1);
    }

    value = array;
  }
  else {
    errorMsg("Invalid type name");
    delete exprList;
    return 0;
  }

  delete exprList;

  return value;
}

CJExecReturn *
CJavaScript::
execReturn()
{
  CJExecExpression *expr = execExpression();

  if (! expr)
    return 0;

  CJExecReturn *eret = new CJExecReturn(CJExecExpressionP(expr));

  return eret;
}

CJFunctionP
CJavaScript::
execUserFunction()
{
  CJFunctionP fn;

  CJTokenP token = execData_->token();

  if (token->type() != CJToken::Type::Identifier) {
    errorMsg("Missing identifier");
    return fn;
  }

  std::string name = token->cast<CJIdentifier>()->name();

  execData_->next();

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg("Missing open bracket for function");
    return fn;
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
      return fn;
    }

    execData_->next();

    token = execData_->token();

    if (token->type() != CJToken::Type::Identifier) {
      errorMsg("Missing arg after comma for function args");
      return fn;
    }
  }

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg("Missing close bracket for function");
    return fn;
  }

  execData_->next();

  CJExecBlock *block = execBlock(CJExecBlock::Type::Function);

  if (! block)
    return fn;

  block->interp(this);

  CJUserFunction *userFn = new CJUserFunction(name, args, CJExecBlockP(block));

  fn = CJFunctionP(userFn);

  addFunction(fn);

  return fn;
}

CJExecExpressionList *
CJavaScript::
execExpressionList()
{
  CJExecExpressionList *exprList = new CJExecExpressionList;

  while (! execData_->eof()) {
    CJExecExpression *expr = execExpression();

    if (! expr) {
      delete exprList;
      return 0;
    }

    exprList->addExpression(CJExecExpressionP(expr));

    if (! isExecOperator(CJOperator::Type::Comma))
      break;

    execData_->next();
  }

  if (isExecOperator(CJOperator::Type::SemiColon))
    execData_->next();

  return exprList;
}

CJExecExpression *
CJavaScript::
execExpression()
{
  CJExecExpression *expr = new CJExecExpression;

  while (! execData_->eof()) {
    CJTokenP token = execData_->token();

    if      (token->type() == CJToken::Type::Identifier) {
      CJExecIdentifiers *identifiers = execIdentfiers();

      if      (isExecOperator(CJOperator::Type::OpenRBracket)) {
        CJExecFunction *function = execFunction(CJExecIdentifiersP(identifiers));

        if (! function) {
          delete expr;
          return 0;
        }

        expr->addToken(CJTokenP(function));
      }
      else if (isExecOperator(CJOperator::Type::OpenSBracket)) {
        execData_->next();

        CJExecIndexExpression *iexpr = new CJExecIndexExpression;

        iexpr->setIdentifiers(CJExecIdentifiersP(identifiers));

        CJExecExpression *expr1 = execExpression();

        if (expr1)
          iexpr->setIndexExpression(CJExecExpressionP(expr1));

        if (! isExecOperator(CJOperator::Type::CloseSBracket)) {
          errorMsg("Missing close square bracket");
          delete iexpr;
          return 0;
        }

        execData_->next();

        expr->addToken(CJTokenP(iexpr));
      }
      else {
        CJTokenP token1 = execData_->token();

        if (isOperator(token1, CJOperator::Type::Increment) ||
            isOperator(token1, CJOperator::Type::Decrement)) {
          execData_->next();

          CJExecIncrDecrExpression *incrDecr = new CJExecIncrDecrExpression;

          CJOperator *op1 = token1->cast<CJOperator>();

          incrDecr->setIdentifiers(CJExecIdentifiersP(identifiers));
          incrDecr->setOperator   (CJOperatorP(op1->dup()));
          incrDecr->setPost       (true);

          expr->addToken(CJTokenP(incrDecr));
        }
        else
          expr->addToken(CJTokenP(identifiers));
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

        CJExecExpression *expr1 = execExpression();

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
          CJExecIdentifiers *identifiers = execIdentfiers();

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

        CJExecQuestion *equestion = execQuestion(expr);

        if (! equestion) {
          errorMsg("Exec failed for '?'");
          break;
        }

        execData_->addEToken(CJTokenP(equestion));
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

        CJExecExpression *expr1 = execExpression();

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
        execData_->next();

        CJExecIndexExpression *iexpr = new CJExecIndexExpression;

        iexpr->setExpression(CJExecExpressionP(expr));

        CJExecExpression *expr1 = execExpression();

        if (expr1)
          iexpr->setIndexExpression(CJExecExpressionP(expr1));

        if (! isExecOperator(CJOperator::Type::CloseSBracket)) {
          errorMsg("Missing close square bracket");
          delete iexpr;
          return 0;
        }

        execData_->next();

        return iexpr;
      }
      else if (opType == CJOperator::Type::CloseSBracket) {
        return expr;
      }
      else if (opType == CJOperator::Type::OpenBrace) {
        CJExecBlock *block = execBlock(CJExecBlock::Type::Sequential);

        if (! block)
          return 0;

        block->interp(this);

        expr->addToken(CJTokenP(block));
      }
      else {
        execData_->next();

        expr->addToken(token);
      }
    }
    else if (token->type() == CJToken::Type::Keyword) {
      CJKeyword *keyword = token->cast<CJKeyword>();

      if (keyword->type() == CJKeyword::Type::New) {
        execData_->next();

        CJValue *value = execNew();

        if (! value) {
          errorMsg("Exec failed for 'new'");
          break;
        }

        expr->addToken(CJTokenP(value));
      }
      else {
        errorMsg("Exec failed at keyword: " + keyword->name());
        break;
      }
    }
    else {
      std::stringstream ss; ss << *token;
      errorMsg("Exec failed at token: " + ss.str());
      delete expr;
      return 0;
    }
  }

  return expr;
}

CJExecIdentifiers *
CJavaScript::
execIdentfiers()
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

  return identifiers;
}

CJExecBlock *
CJavaScript::
execBlock(CJExecBlock::Type type)
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
  switch (op) {
    case CJOperator::Type::Plus:
      if      (value1->type() == CJValue::Type::String ||
               value2->type() == CJValue::Type::String) {
        std::string s1 = value1->toString();
        std::string s2 = value2->toString();

        return CJValueP(new CJString(s1 + s2));
      }
      else if (value1->type() == CJValue::Type::Number &&
               value2->type() == CJValue::Type::Number) {
        double r1 = value1->cast<CJNumber>()->real();
        double r2 = value2->cast<CJNumber>()->real();

        return CJValueP(new CJNumber(r1 + r2));
      }

      break;
    case CJOperator::Type::Minus:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        double r1 = value1->cast<CJNumber>()->real();
        double r2 = value2->cast<CJNumber>()->real();

        return CJValueP(new CJNumber(r1 - r2));
      }

      break;
    case CJOperator::Type::Times:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        double r1 = value1->cast<CJNumber>()->real();
        double r2 = value2->cast<CJNumber>()->real();

        return CJValueP(new CJNumber(r1 * r2));
      }

      break;
    case CJOperator::Type::Divide:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        double r1 = value1->cast<CJNumber>()->real();
        double r2 = value2->cast<CJNumber>()->real();

        if (r2 != 0)
          return CJValueP(new CJNumber(r1 / r2));
        else
          return CJValueP(new CJNumber(getNaN()));
      }

      break;
    case CJOperator::Type::Modulus:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        double r1 = value1->cast<CJNumber>()->real();
        double r2 = value2->cast<CJNumber>()->real();

        return CJValueP(new CJNumber(realModulus(r1, r2)));
      }

      break;

    case CJOperator::Type::Less:
      return boolValue(cmp(value1, value2) <  0);
    case CJOperator::Type::Greater:
      return boolValue(cmp(value1, value2) >  0);
    case CJOperator::Type::LessEqual:
      return boolValue(cmp(value1, value2) <= 0);
    case CJOperator::Type::GreaterEqual:
      return boolValue(cmp(value1, value2) >= 0);
    case CJOperator::Type::Equal:
      return boolValue(cmp(value1, value2) == 0);
    case CJOperator::Type::NotEqual:
      return boolValue(cmp(value1, value2) != 0);

    case CJOperator::Type::LogicalAnd:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        return CJValueP(new CJNumber(i1 && i2));
      }

      break;
    case CJOperator::Type::LogicalOr:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        return CJValueP(new CJNumber(i1 || i2));
      }

      break;

    case CJOperator::Type::BitwiseAnd:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        return CJValueP(new CJNumber(i1 & i2));
      }

      break;
    case CJOperator::Type::BitwiseOr:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        return CJValueP(new CJNumber(i1 | i2));
      }

      break;
    case CJOperator::Type::BitwiseXor:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        return CJValueP(new CJNumber(i1 ^ i2));
      }

      break;

    case CJOperator::Type::LeftShift:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        return CJValueP(new CJNumber(i1 << i2));
      }

      break;
    case CJOperator::Type::RightShift:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        return CJValueP(new CJNumber(i1 >> i2));
      }

      break;
    case CJOperator::Type::UnsignedRightShift:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        ulong i1 = value1->toInteger();
        long  i2 = value2->toInteger();

        return CJValueP(new CJNumber(i1 >> i2));
      }

      break;

    default:
      break;
  }

  return CJValueP();
}

CJValueP
CJavaScript::
execUnaryOp(CJOperator::Type op, CJValueP value)
{
  assert(value);

  switch (op) {
    case CJOperator::Type::Increment:
      if (value->type() == CJValue::Type::Number) {
        double r = value->cast<CJNumber>()->real();

        return CJValueP(new CJNumber(r + 1));
      }

      break;
    case CJOperator::Type::Decrement:
      if (value->type() == CJValue::Type::Number) {
        double r = value->cast<CJNumber>()->real();

        return CJValueP(new CJNumber(r - 1));
      }

      break;

    case CJOperator::Type::LogicalNot:
      if (value->type() == CJValue::Type::Number) {
        long i = value->toInteger();

        return CJValueP(new CJNumber(! i));
      }

      break;

    case CJOperator::Type::BitwiseNot:
      if (value->type() == CJValue::Type::Number) {
        long i = value->toInteger();

        return CJValueP(new CJNumber(~ i));
      }

      break;

    default:
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
    tokens_.push_back(CJTokenP(new CJNull()));
  else if (name == "true")
    tokens_.push_back(CJTokenP(new CJTrue()));
  else if (name == "false")
    tokens_.push_back(CJTokenP(new CJFalse()));
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
    keywords["continue" ] = CJKeyword::Type::Continue;
    keywords["delete"   ] = CJKeyword::Type::Delete;
    keywords["else"     ] = CJKeyword::Type::Else;
    keywords["for"      ] = CJKeyword::Type::For;
    keywords["function" ] = CJKeyword::Type::Function;
    keywords["if"       ] = CJKeyword::Type::If;
    keywords["in"       ] = CJKeyword::Type::In;
    keywords["new"      ] = CJKeyword::Type::New;
    keywords["return"   ] = CJKeyword::Type::Return;
    keywords["this"     ] = CJKeyword::Type::This;
    keywords["typeof"   ] = CJKeyword::Type::TypeOf;
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

        octValue = 16*octValue + (c - '0');
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

  CJNumber *number = 0;

  if      (isOct)
    number = new CJNumber(sign*octValue);
  else if (isHex)
    number = new CJNumber(sign*hexValue);
  else {
    const char *p;

    double real = strtod(realStr.c_str(), (char **) &p);

    number = new CJNumber(sign*real);
  }

  tokens_.push_back(CJTokenP(number));
}

void
CJavaScript::
readOperator(CStrParse &parse)
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
        type       = CJOperator::Type::Plus;
        precedence = 3;
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
        type       = CJOperator::Type::Minus;
        precedence = 3;
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
          case 'f': str += '\f'; break;
          case 'n': str += '\n'; break;
          case 'r': str += '\r'; break;
          case 't': str += '\t'; break;
          case '\'': str += '\''; break;
          case '\"': str += '\"'; break;
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

  tokens_.push_back(CJTokenP(new CJString(str, '\"')));
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

  tokens_.push_back(CJTokenP(new CJString(str, '\'')));
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
    default                       : return "<none>";
    case CJKeyword::Type::Break   : return "break";
    case CJKeyword::Type::Continue: return "continue";
    case CJKeyword::Type::Delete  : return "delete";
    case CJKeyword::Type::Else    : return "else";
    case CJKeyword::Type::For     : return "for";
    case CJKeyword::Type::Function: return "function";
    case CJKeyword::Type::If      : return "if";
    case CJKeyword::Type::In      : return "in";
    case CJKeyword::Type::New     : return "new";
    case CJKeyword::Type::Return  : return "return";
    case CJKeyword::Type::This    : return "this";
    case CJKeyword::Type::TypeOf  : return "typeof";
    case CJKeyword::Type::Var     : return "var";
    case CJKeyword::Type::Void    : return "void";
    case CJKeyword::Type::While   : return "while";
    case CJKeyword::Type::With    : return "with";
  }
}

//------

std::string
CJOperator::
name() const
{
  switch (type_) {
    default                            : return "<none>";
    case Type::Plus                    : return "+";
    case Type::PlusAssign              : return "+=";
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
  }
}

//------

CJValueP
CJVariable::
value(bool create)
{
  if (! value_ && create)
    value_ = CJValueP(new CJNumber(0));

  return value_;
}

//------

void
CJScope::
addScope(CJScopeP scope)
{
  scopes_[scope->name()] = scope;
}

void
CJScope::
addFunction(CJFunctionP fn)
{
  functions_[fn->name()] = fn;
}

void
CJScope::
addVariable(CJVariableP var)
{
  variables_[var->name()] = var;
}

void
CJScope::
addObject(CJObjectP obj)
{
  objects_[obj->name()] = obj;
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

CJFunctionP
CJScope::
lookupFunction(const std::string &name)
{
  auto p = functions_.find(name);

  if (p == functions_.end())
    return CJFunctionP();

  return (*p).second;
}

CJVariableP
CJScope::
lookupVariable(const std::string &name, bool create)
{
  auto p = variables_.find(name);

  if (p != variables_.end())
    return (*p).second;

  if (! create)
    return CJVariableP();

  CJVariable *var = new CJVariable(name);

  CJVariableP pvar(var);

  addVariable(pvar);

  return pvar;
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

  if (! varToken || varToken->type() != CJToken::Type::Identifiers) {
    js->errorMsg("Missing variable name for assign");
    return value;
  }

  CJVariableP var =
    js->lookupVariable(varToken->cast<CJExecIdentifiers>()->identifiers(), /*create*/true);

  CJValueP rvalue = rexpr_->exec(js);

  if (! rvalue)
    return value;

  CJOperator::Type opType = op_->type();

  switch (opType) {
    case CJOperator::Type::Assign:
      break;
    case CJOperator::Type::PlusAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Plus, var->value(), rvalue);
      break;
    case CJOperator::Type::MinusAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Minus, var->value(), rvalue);
      break;
    case CJOperator::Type::TimesAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Times, var->value(), rvalue);
      break;
    case CJOperator::Type::DivideAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Divide, var->value(), rvalue);
      break;
    case CJOperator::Type::ModulusAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::Modulus, var->value(), rvalue);
      break;
    case CJOperator::Type::LeftShiftAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::LeftShift, var->value(), rvalue);
      break;
    case CJOperator::Type::RightShiftAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::RightShift, var->value(), rvalue);
      break;
    case CJOperator::Type::UnsignedRightShiftAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::UnsignedRightShift, var->value(), rvalue);
      break;
    case CJOperator::Type::BitwiseAndAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseAnd, var->value(), rvalue);
      break;
    case CJOperator::Type::BitwiseOrAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseOr, var->value(), rvalue);
      break;
    case CJOperator::Type::BitwiseNotAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseNot, var->value(), rvalue);
      break;
    case CJOperator::Type::BitwiseXorAssign:
      rvalue = js->execBinaryOp(CJOperator::Type::BitwiseXor, var->value(), rvalue);
      break;
    default:
      break;
  }

  var->setValue(rvalue);

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
    CJVariableP var = js->lookupVariable(identifiers_->identifiers(), /*create*/false);

    if (var)
      avalue = var->value();
  }
  else {
    avalue = expr_->exec(js);
  }

  if (! avalue || avalue->type() != CJToken::Type::Array) {
    js->errorMsg("Value not an array for index");
    return value;
  }

  CJArray *array = avalue->cast<CJArray>();

  CJValueP ivalue = iexpr_->exec(js);

  int ind = ivalue->toInteger();

  return array->indexValue(ind);
}

//------

CJValueP
CJExecIncrDecrExpression::
exec(CJavaScript *js)
{
  CJValueP value;

  if (! identifiers_ || ! op_)
    return value;

  CJVariableP var = js->lookupVariable(identifiers_->identifiers(), /*create*/true);

  CJOperator::Type opType = op_->type();

  if (var)
    value = var->value();

  CJValueP value1 = js->execUnaryOp(opType, value);

  var->setValue(value1);

  return (post_ ? value : value1);
}

//------

void
CJExecBlock::
interp(CJavaScript *js)
{
  CJavaScript::ExecData execData;

  execData.initExec(tokens_);

  js->interp(execData);

  etokens_ = execData.etokens();
}

CJValueP
CJExecBlock::
exec(CJavaScript *js)
{
  CJValueP value;

  for (auto &e : etokens_) {
    value = e->exec(js);

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

    CJToken::Type type = token->type();

    if      (token->isValue()) {
      CJValueP value(token.get()->cast<CJValue>()->dup());

      values.push_back(value);
    }
    else if (type == CJToken::Type::Operator) {
      CJOperator *op = token->cast<CJOperator>();

      bool unstack = false;

      if (lastOp) {
        if      (lastOp->precedence() > op->precedence())
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
      CJVariableP var =
        js->lookupVariable(token->cast<CJExecIdentifiers>()->identifiers(), /*create*/true);

      if (var) {
        CJValueP value = var->value();

        if (value)
          values.push_back(value);
        else
          js->errorMsg("Invalid variable value");
      }
      else {
        CJValueP value = js->variableProperty(token->cast<CJExecIdentifiers>()->identifiers());

        if (value)
          values.push_back(value);
        else
          js->errorMsg("Invalid variable property value");
      }
    }
    else {
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

  CJFunctionP fn = js->lookupFunction(identifiers_->identifiers());

  if (! fn) {
    auto fnValue = js->variableFunction(identifiers_->identifiers());

    if (! fnValue.first) {
      std::stringstream ss; ss << *identifiers_;
      js->errorMsg("Function not found : " + ss.str());
      return CJValueP();
    }

    fn = fnValue.first;

    values.push_back(fnValue.second);
  }

  for (auto &e : exprList_->expressions()) {
    CJValueP value = e->exec(js);

    if (value)
      values.push_back(value);
  }

  if (! fn) {
    std::stringstream ss; ss << *identifiers_;
    js->errorMsg("Function not found : " + ss.str());
    return CJValueP();
  }

  return fn->exec(js, values);
}

//------

CJValueP
CJExecFor::
exec(CJavaScript *js)
{
  if (! exprList1_ || ! exprList2_ || ! exprList3_ || ! block_)
    return CJValueP();

  (void) exprList1_->exec(js);

  CJValueP value2 = exprList2_->exec(js);

  while (value2 && value2->toBoolean()) {
    js->startBlock(block_);

    block_->exec(js);

    bool breakFlag = block_->isBreakFlag();

    js->endBlock();

    if (breakFlag)
      break;

    (void) exprList3_->exec(js);

    value2 = exprList2_->exec(js);
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
  CJValueP value;

  for (const auto &ie : idExprList_) {
    CJExecIdentifiersP    identifiers = ie.first;
    CJExecExpressionListP expr        = ie.second;

    CJVariableP var = js->lookupVariable(identifiers->identifiers(), /*create*/true);

    if (expr) {
      CJValueP evalue = expr->exec(js);

      if (var && evalue)
        var->setValue(evalue);
    }

    value = var->value();
  }

  return 0;
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
CJRealFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 1) {
    js->errorMsg("Wrong number of function values");
    return CJValueP();
  }

  double r = values[0]->toReal();

  double res = (*fn_)(r);

  return CJValueP(new CJNumber(res));
}

//------

CJValueP
CJUserFunction::
exec(CJavaScript *js, const Values &values)
{
  CJScopeP scope = CJScopeP(new CJScope(name_));

  if (values.size() != args_.size()) {
    js->errorMsg("Wrong number of function values");
    return CJValueP();
  }

  int nv = values.size();

  for (int i = 0; i < nv; ++i) {
    CJVariableP var = CJVariableP(new CJVariable(args_[i]));

    var->setValue(values[i]);

    scope->addVariable(var);
  }

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

  CJObject *obj = values[0].get()->cast<CJObject>();

  CJValueP value = obj->execNameFn(js, name(), values);

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
exec(CJavaScript *, const Values &values)
{
  if (values.size() != 2)
    return CJValueP();

  // create timer to run proc every n miliseconds
  std::string proc     = values[0]->toString();
  double      interval = values[1]->toReal();

std::cerr << "setInterval: " << proc << " " << interval << std::endl;
  return CJValueP(new CJNumber(0));
}

CJValueP
CJClearInterval::
exec(CJavaScript *, const Values &values)
{
  if (values.size() != 1)
    return CJValueP();

  int timer = values[0]->toInteger();

std::cerr << "clearInterval: " << timer << std::endl;
  return CJValueP();
}

//------

CJValueP
CJDocumentWriteFunction::
exec(CJavaScript *, const Values &values)
{
  int i = 0;

  for (auto &v : values) {
    if (i > 0) std::cout << " ";

    std::cout << *v;

    ++i;
  }

  std::cout << std::endl;

  return CJValueP();
}

//------

CJValueP
CJStringCharAtFunction::
exec(CJavaScript *, const Values &values)
{
  if (values.size() != 2)
    return CJValueP();

  CJString *str = values[0]->cast<CJString>();

  if (! str)
    return CJValueP();

  long ind = values[1]->toInteger();

  if (ind < 0 || ind >= long(str->text().size()))
    return CJValueP();

  return CJValueP(new CJString(str->text().substr(ind, 1)));
}

CJValueP
CJStringIndexOfFunction::
exec(CJavaScript *, const Values &values)
{
  if (values.size() != 2)
    return CJValueP();

  CJString *str = values[0]->cast<CJString>();

  if (! str)
    return CJValueP();

  CJString *key = values[1]->cast<CJString>();

  if (! key)
    return CJValueP();

  auto p = str->text().find(key->text());

  if (p == std::string::npos)
    return CJValueP(new CJNumber(-1));

  return CJValueP(new CJNumber(p));
}

CJValueP
CJStringLengthFunction::
exec(CJavaScript *, const Values &values)
{
  if (values.size() != 1)
    return CJValueP();

  CJString *str = values[0]->cast<CJString>();

  if (! str)
    return CJValueP();

  return CJValueP(new CJNumber(str->text().size()));
}

CJValueP
CJStringSubStringFunction::
exec(CJavaScript *, const Values &values)
{
  if (values.size() != 3)
    return CJValueP();

  CJString *str = values[0]->cast<CJString>();

  if (! str)
    return CJValueP();

  long ind1 = values[1]->toInteger();
  long ind2 = values[2]->toInteger();

  if ((ind1 < 0 || ind1 >= long(str->text().size())) ||
      (ind2 < 0 || ind2 >= long(str->text().size())) ||
      ind1 > ind2)
    return CJValueP();

  return CJValueP(new CJString(str->text().substr(ind1, ind2 - ind1)));
}

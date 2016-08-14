#include <CJavaScript.h>
#include <CJGlobalFunction.h>
#include <CJExecThis.h>
#include <CJDocument.h>
#include <CJConsole.h>
#include <CJDebug.h>
#include <CJUtil.h>
#include <CFile.h>
#include <CStrUtil.h>
#include <CStrParse.h>
#include <iostream>
#include <cmath>

//---

CJavaScript::
CJavaScript()
{
  // Types (prototype ?)
  rootScope_    = CJDictionaryP(new CJDictionary(this, "__root__"));
  currentScope_ = rootScope_;

  //------

  // Global
  setProperty("alert"        , CJValueP(new CJAlertFunction(this)));
  setProperty("setInterval"  , CJValueP(new CJSetInterval  (this)));
  setProperty("clearInterval", CJValueP(new CJClearInterval(this)));

  setProperty("undefined", CJValueP(createUndefinedValue()));
  setProperty("null"     , CJValueP(createNullValue()));

  setProperty("NaN"     , CJValueP(new CJNumber(this, CJUtil::getNaN())));
  setProperty("Infinity", CJValueP(new CJNumber(this, CJUtil::getPosInf())));

  //------

  // Math Dictionary
  // TODO: mark as internal ? different behavior than user dictionary
  CJDictionaryP mathDictionary(new CJDictionary(this));

  setProperty("Math", std::static_pointer_cast<CJValue>(mathDictionary));

  double Eval = exp(1);

  mathDictionary->setRealProperty(this, "E"      , Eval);
  mathDictionary->setRealProperty(this, "LN10"   , log(10));
  mathDictionary->setRealProperty(this, "LN2"    , log(2));
  mathDictionary->setRealProperty(this, "LN10E"  , log10(Eval));
  mathDictionary->setRealProperty(this, "LN2E"   , log2(Eval));
  mathDictionary->setRealProperty(this, "PI"     , M_PI);
  mathDictionary->setRealProperty(this, "SQRT1_2", sqrt(1.0/2.0));
  mathDictionary->setRealProperty(this, "SQRT2"  , sqrt(2));

  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "abs"   , fabs )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "acos"  , acos )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "acosh" , acosh)));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "asin"  , asin )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "asinh" , asinh)));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "atan"  , atan )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJReal2Function(this, "atan2" , atan2)));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "ceil"  , ceil )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "cos"   , cos  )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "cosh"  , cosh )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "exp"   , exp  )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "floor" , floor)));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJReal2Function(this, "hypot" , hypot)));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "log"   , log  )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "log10" , log10)));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "log2"  , log2 )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJReal2Function(this, "max"   , CJUtil::max)));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJReal2Function(this, "min"   , CJUtil::min)));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJReal2Function(this, "pow"   , pow  )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRandFunction (this)));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "round" , round)));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "sign"  , CJUtil::sign)));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "sin"   , sin  )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "sinh"  , sinh )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "sqrt"  , sqrt )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "tan"   , tan  )));
  mathDictionary->setFunctionProperty(this,
    CJFunctionP(new CJRealFunction (this, "tanh"  , tanh )));

  //------

  setProperty("Number" , CJValueP(new CJNumberFunction (this)));
  setProperty("String" , CJValueP(new CJStringFunction (this)));
  setProperty("Boolean", CJValueP(new CJBooleanFunction(this)));
  setProperty("Array"  , CJValueP(new CJArrayFunction  (this)));
  setProperty("Object" , CJValueP(new CJObjectFunction (this)));

  //------

  setProperty("eval"      , CJValueP(new CJGlobalFunction(this, "eval"      )));
//setProperty("isFinite"  , CJValueP(new CJGlobalFunction(this, "isFinite"  )));
  setProperty("isNaN"     , CJValueP(new CJGlobalFunction(this, "isNaN"     )));
  setProperty("parseInt"  , CJValueP(new CJGlobalFunction(this, "parseInt"  )));
  setProperty("parseFloat", CJValueP(new CJGlobalFunction(this, "parseFloat")));

  //------

  // document object
  setProperty("document", CJValueP(new CJDocumentObject(this)));

  // console object
  setProperty("console", CJValueP(new CJConsoleObject(this)));

  // debug object
  setProperty("debug", CJValueP(new CJDebugObject(this)));
}

CJObjTypeP
CJavaScript::
addObjectType(const std::string &name, CJObjTypeP type)
{
  namedType_[name] = type;

  return type;
}

CJObjTypeP
CJavaScript::
getObjectType(const std::string &name) const
{
  auto p = namedType_.find(name);

  if (p != namedType_.end())
    return (*p).second;

  return CJObjTypeP();
}

void
CJavaScript::
startScope(CJDictionaryP scope)
{
  scopeStack_.push_back(currentScope_);

  scope->setParent(currentScope_);

  currentScope_ = scope;
//printScopeStack("startScope");
}

CJDictionaryP
CJavaScript::
endScope()
{
  CJDictionaryP scope = currentScope_;

  currentScope_ = scopeStack_.back();

  scopeStack_.pop_back();

  scope->setParent(0);

//printScopeStack("endScope");
  return scope;
}

void
CJavaScript::
startFunctionScope(CJUserFunctionP fn)
{
  functions_.push_back(fn);

  CJDictionaryP scope = fn->scope();

  if (getenv("JS_PRINT_FUNCTION_SCOPE"))
    printScopeChain(scope, "startFunctionScope [proc " + scope->name() + "]");

  currentScopeStack_.push_back(currentScope_);
  scopeStackStack_  .push_back(scopeStack_);

  initFunctionScope(fn);

  currentScope_ = fn->scope();

  scopeStack_.clear();

  if (getenv("JS_PRINT_FUNCTION_SCOPE"))
    printScopeStack("startFunctionScope [stack]");
}

CJUserFunctionP
CJavaScript::
endFunctionScope()
{
  if (getenv("JS_PRINT_FUNCTION_SCOPE"))
    printScopeStack("endFunctionScope [stack]");

  CJDictionaryP scope = currentScope_;

  currentScope_ = currentScopeStack_.back();
  scopeStack_   = scopeStackStack_  .back();

  currentScopeStack_.pop_back();
  scopeStackStack_  .pop_back();

  CJUserFunctionP fn = functions_.back();

  functions_.pop_back();

  termFunctionScope(fn);

  if (! functions_.empty()) {
    CJUserFunctionP fn = functions_.back();

    initFunctionScope(fn);
  }

  if (getenv("JS_PRINT_FUNCTION_SCOPE"))
    printScopeChain(currentScope_, "endFunctionScope [proc " + scope->name() + "]");

  return fn;
}

void
CJavaScript::
initFunctionScope(CJUserFunctionP fn)
{
  CJDictionaryP rootScope = fn->scope();

  while (rootScope->getParent())
    rootScope = rootScope->getParent();

  if (rootScope.get() != rootScope_.get())
    rootScope->setParent(rootScope_);
}

void
CJavaScript::
termFunctionScope(CJUserFunctionP fn)
{
  CJDictionaryP rootScope = fn->scope();

  while (rootScope->getParent())
    rootScope = rootScope->getParent();

  rootScope->setParent(CJDictionaryP());
}

CJUserFunctionP
CJavaScript::
currentFunction() const
{
  if (functions_.empty())
    return CJUserFunctionP();

  return functions_.back();
}

void
CJavaScript::
printScopeStack(const std::string &msg) const
{
  std::cout << msg << " " << currentScope_->name();

  for (const auto &s : scopeStack_)
    std::cout << ":" << s->name();

  std::cout << std::endl;
}

void
CJavaScript::
printScopeChain(CJDictionaryP scope, const std::string &msg) const
{
  std::cout << msg << " ";

  int i = 0;

  CJDictionaryP scope1 = scope;

  while (scope1) {
    if (i > 0)
      std::cout << ":";

    std::cout << scope1->name();

    scope1 = scope1->getParent();

    ++i;
  }

  std::cout << std::endl;
}

void
CJavaScript::
pushUserFunction(CJUserFunctionP fn)
{
  userFunctions_.push_back(fn);
}

void
CJavaScript::
popUserFunction()
{
  userFunctions_.pop_back();
}

void
CJavaScript::
printUserFunctions(const std::string &msg) const
{
  std::cout << msg << " " << currentScope()->name();

  for (const auto &f : userFunctions_)
    std::cout << ":" << f->name();

  std::cout << std::endl;
}

std::vector<std::string>
CJavaScript::
getFunctionNames() const
{
  return currentScope()->getFunctionNames();
}

std::vector<std::string>
CJavaScript::
getVariableNames() const
{
  return currentScope()->getVariableNames();
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

    if (block->isReturnFlag()) {
      block_->setReturnFlag(true);
      block_->setRetVal(block->getRetVal());
    }
  }
  else if (block->type() == CJExecBlock::Type::Iterative) {
    if (block->getRetVal())
      block_->setRetVal(block->getRetVal());
  }

  return block;
}

CJValueP
CJavaScript::
setProperty(const std::string &name, CJValueP value)
{
  currentScope()->setProperty(name, value);

  return value;
}

//---

bool
CJavaScript::
lookupPropertyData(const Identifiers &identifiers, CJPropertyData &data)
{
  bool create = false;

  std::swap(data.create, create);

  CJDictionaryP scope = currentScope();

  if (lookupPropertyData(scope, identifiers, data, 0))
    return true;

  while (scope->getParent()) {
    scope = scope->getParent();

    if (lookupPropertyData(scope, identifiers, data, 0))
      return true;
  }

  if (create) {
    std::swap(data.create, create);

    CJDictionaryP scope = currentScope();

    if (lookupPropertyData(scope, identifiers, data, 0))
      return true;

    while (scope->getParent()) {
      scope = scope->getParent();

      if (lookupPropertyData(scope, identifiers, data, 0))
        return true;
    }
  }

  return false;
}

bool
CJavaScript::
lookupPropertyData(CJDictionaryP scope, const Identifiers &identifiers,
                   CJPropertyData &data, int ind)
{
  int len = identifiers.size();

  if (ind >= len)
    return false;

  const std::string &name = identifiers[ind]->name();

  if (ind < len - 1) {
    // check named scope
    //CJDictionaryP scope1 = scope->lookupScope(name);

    //if (scope1) {
    //  return lookupPropertyData(scope1, identifiers, data, ind + 1);
    //}

    //---

    // get property
    CJValueP value = scope->getProperty(name);

    if (! value)
      return false;

    // lookup in dictionary
    return lookupPropertyData(value, identifiers, data, ind + 1);
  }
  else {
    // get property
    CJValueP value = scope->getProperty(name);

    if (! value) {
      if (! data.create)
        return false;

//std::cerr << "Create scope '" << scope->name() << "' property '" << name << std::endl;
      value = CJValueP(createNullValue());

      scope->setProperty(name, value);
    }

    data.dict     = scope;
    data.objValue = scope;
    data.value    = value;

    // lvalue
    if (data.modifiable)
      data.lvalue = CJLValueP(new CJDictionaryValue(this, scope, name));

    return true;
  }
}

bool
CJavaScript::
lookupPropertyData(CJObjP obj, const Identifiers &identifiers,
                   CJPropertyData &data, int ind)
{
  int len = identifiers.size();

  if (ind >= len)
    return false;

  const std::string &name = identifiers[ind]->name();

  if (ind < len - 1) {
    // get object value
    CJValueP value = obj->getProperty(name);

    if (! value)
      return false;

    return lookupPropertyData(value, identifiers, data, ind + 1);
  }
  else {
    data.obj      = obj;
    data.objValue = obj;

    // lvalue
    if (data.modifiable)
      data.lvalue = CJLValueP(new CJObjectValue(this, obj, name));
    else {
      data.value = obj->getProperty(name);

      if (! data.value) {
        CJObjTypeP objType = obj->type();

        if (objType->hasProperty(name)) {
          data.value = objType->getProperty(name);
        }
      }
    }

    return true;
  }
}

bool
CJavaScript::
lookupPropertyData(CJValueP value, const Identifiers &identifiers,
                   CJPropertyData &data, int ind)
{
  int len = identifiers.size();

  if (ind >= len)
    return false;

  // lookup in dictionary
  if      (value->type() == CJToken::Type::Dictionary) {
    CJDictionaryP dict1 = std::static_pointer_cast<CJDictionary>(value);

    return lookupPropertyData(dict1, identifiers, data, ind);
  }
  // lookup in object
  else if (value->isObject()) {
    CJObjP obj1 = std::static_pointer_cast<CJObj>(value);

    return lookupPropertyData(obj1, identifiers, data, ind);
  }
  else if (value->type() == CJToken::Type::Function) {
    CJFunctionP fn1 = std::static_pointer_cast<CJFunction>(value);

    if      (fn1->type() == CJFunction::Type::User) {
      CJUserFunctionP userFn1 = std::static_pointer_cast<CJUserFunction>(value);

      return lookupPropertyData(userFn1->scope(), identifiers, data, ind);
    }
    else if (fn1->type() == CJFunction::Type::Type ||
             fn1->type() == CJFunction::Type::ObjectType ||
             fn1->type() == CJFunction::Type::Object) {
      if (ind != len - 1) {
        errorMsg("Invalid object type identifiers");
        return false;
      }

      CJObjTypeP valueType1;

      if (fn1->type() == CJFunction::Type::Type) {
        CJTypeFunctionP typeFn = std::static_pointer_cast<CJTypeFunction>(value);

        valueType1 = typeFn->objectType();
      }
      if (fn1->type() == CJFunction::Type::ObjectType) {
        CJObjectTypeFunctionP objTypeFn = std::static_pointer_cast<CJObjectTypeFunction>(value);

        valueType1 = objTypeFn->objectType();
      }
      else {
        //CJObjectFunctionP objFn = std::static_pointer_cast<CJObjectFunction>(value);
      }

      const std::string &name1 = identifiers[ind]->name();

      CJValueP propVal;

      if (valueType1 && valueType1->hasProperty(name1))
        propVal = valueType1->getProperty(name1);

      if (! propVal)
        return false;

      if (propVal->type() == CJToken::Type::Function) {
        CJTypeObjectFunctionP fn(new CJTypeObjectFunction(this, valueType1, name1));

        data.objValue = value;
        data.value    = std::static_pointer_cast<CJValue>(fn);

        return true;
      }
      else {
        // TODO: namespace function
        data.value = propVal;

#if 0
        CJObjType::Values values;

        values.push_back(value);

        data.value = valueType1->exec(this, name1, values);
#endif

        return true;
      }
    }
    else
      return false;
  }
  // lookup object property (TODO: support multiple names)
  else {
    // value is an object
    if (ind != len - 1) {
      errorMsg("Invalid object identifiers");
      return false;
    }

    const std::string &name1 = identifiers[ind]->name();

    CJValueP propVal;

    CJObjTypeP valueType = value->valueType();

    if (valueType->hasProperty(name1))
      propVal = valueType->getProperty(name1);

    if (! propVal)
      return false;

    if (propVal->type() == CJToken::Type::Function) {
      CJTypeObjectFunctionP fn(new CJTypeObjectFunction(this, valueType, name1));

      data.objValue = value;
      data.value    = std::static_pointer_cast<CJValue>(fn);

      return true;
    }
    else {
      CJObjType::Values values;

      values.push_back(value);

      data.value = valueType->exec(this, name1, values);

      return true;
    }
  }
}

//---

CJValueP
CJavaScript::
lookupValue(const Identifiers &identifiers)
{
  CJPropertyData data;

  if (! lookupPropertyData(identifiers, data))
    return CJValueP();

  if (! data.value && data.lvalue)
    return data.lvalue->value();
  else
    return data.value;
}

//---

CJLValueP
CJavaScript::
lookupLValue(const Identifiers &identifiers)
{
  CJPropertyData data;

  data.modifiable = true;

  if (! lookupPropertyData(identifiers, data))
    return CJLValueP();

  return data.lvalue;
}

//---

CJavaScript::ValuePair
CJavaScript::
lookupObjectProperty(const Identifiers &identifiers, bool create)
{
  CJPropertyData data;

  data.modifiable = true;
  data.create     = create;

  if (! lookupPropertyData(identifiers, data))
    return ValuePair();

  CJValueP objValue, dataValue;

  if      (data.obj)
    objValue = std::static_pointer_cast<CJValue>(data.obj);
  else if (data.objValue)
    objValue = data.objValue;

  dataValue = data.value;

  if (! dataValue && data.lvalue)
    dataValue = data.lvalue->value();

  return ValuePair(objValue, dataValue);
}

//---

CJLValueP
CJavaScript::
lookupProperty(const Identifiers &identifiers, bool create)
{
  CJPropertyData data;

  data.modifiable = true;
  data.create     = create;

  if (! lookupPropertyData(identifiers, data))
    return CJLValueP();

  return data.lvalue;
}

//---

bool
CJavaScript::
deleteProperty(const Identifiers &identifiers)
{
  CJDictionaryP scope = currentScope();

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
deleteProperty(CJDictionaryP scope, const Identifiers &identifiers)
{
  int len = identifiers.size();

  for (int i = 0; i < len; ++i) {
    if (i < len - 1) {
      CJValueP varValue = scope->getProperty(identifiers[i]->name());

      if (varValue) {
        ++i;

        if (varValue->type() == CJToken::Type::Dictionary) {
          CJDictionaryP dict = std::static_pointer_cast<CJDictionary>(varValue);

          CJValueP value = dict->getProperty(identifiers[i]->name());

          while (value && value->type() == CJToken::Type::Dictionary && i < len - 1) {
            ++i;

            dict = std::static_pointer_cast<CJDictionary>(value);

            value = dict->getProperty(identifiers[i]->name());
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

//---

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
addTypeObject(CJToken::Type type, CJObjTypeP obj)
{
  typeObject_[type] = obj;
}

void
CJavaScript::
loadFile(const std::string &filename)
{
  fileName_ = filename;
  lineNum_  = 1;

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

  loadText(str);
}

void
CJavaScript::
loadString(const std::string &str)
{
  fileName_ = "";
  lineNum_  = 1;

  loadText(str);
}

void
CJavaScript::
loadText(const std::string &str)
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

  skipSpace(parse);

  while (! parse.eof()) {
    CJToken::Type lastType = lastTokenType();

    bool allowUnary = false;

    if      (lastType == CJToken::Type::None)
      allowUnary = true;
    else if (lastType == CJToken::Type::Operator) {
      CJOperatorP op = std::static_pointer_cast<CJOperator>(tokens_.back());

      if (CJOperator::isAllowUnary(op->type()))
        allowUnary = true;
    }

    if      (parse.isString("typeof")) {
      parse.skipChars(6);

      CJOperatorP op(new CJOperator(CJOperator::Type::TypeOf, 1,
        CJOperator::Associativty::Right, CJOperator::Ary::Unary));

      tokens_.push_back(CJTokenP(op));
    }
    else if (parse.isString("instanceof")) {
      parse.skipChars("instanceof");

      CJOperatorP op(new CJOperator(CJOperator::Type::InstanceOf, 11,
        CJOperator::Associativty::Left, CJOperator::Ary::Binary));

      tokens_.push_back(CJTokenP(op));
    }
    else if (parse.isString("NaN")) {
      parse.skipChars("NaN");

      tokens_.push_back(createNumberValue(CJUtil::getNaN()));
    }
    else if (parse.isString("Infinity")) {
      parse.skipChars("Infinity");

      tokens_.push_back(createNumberValue(CJUtil::getPosInf()));
    }
    else if (parse.isString("-Infinity")) {
      parse.skipChars("-Infinity");

      tokens_.push_back(createNumberValue(CJUtil::getNegInf()));
    }
    else if (parse.isAlpha() || parse.isChar('_')) {
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
    else {
      errorMsg("Invalid text: '" + parse.getAt() + "'");
      break;
    }

    skipSpace(parse);
  }
}

void
CJavaScript::
skipSpace(CStrParse &parse)
{
  while (parse.isSpace()) {
    if (parse.isChar('\n'))
      ++lineNum_;

    parse.skipChar();
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
      CJExecExpressionListP exprList = interpExpressionList();

      if (! exprList) {
        std::ostringstream ss; ss << *token;
        errorMsg(token, "Interp failed at token: " + ss.str());
        break;
      }

      execData_->addEToken(std::static_pointer_cast<CJToken>(exprList));
    }
    else if (type == CJToken::Type::Operator) {
      CJOperator *op = token->cast<CJOperator>();

      CJOperator::Type opType = op->type();

      if (opType == CJOperator::Type::Colon) {
        if (! execData_->isBlock()) {
          std::ostringstream ss; ss << *token;
          errorMsg(token, "Interp failed at operator: " + ss.str());
          break;
        }

        execData_->next();

        //---

        CJValueP key = execData_->unstackEValue(this);

        if (! key) {
          errorMsg(token, "Left hand side value missing for colon");
          break;
        }

        CJExecExpressionP valueExpr = interpExpression();

        if (! valueExpr) {
          errorMsg(token, "Missing expression after colon");
          break;
        }

        //---

        CJExecDictionaryP dict(new CJExecDictionary);

        dict->setLineNum(lineNum_);

        dict->addDictionaryValue(key, valueExpr);

        //---

        while (isOperator(execData_->token(), CJOperator::Type::Comma)) {
          execData_->next();

          CJExecExpressionP keyExpr = interpExpression();

          if (! keyExpr) {
            errorMsg(token, "Missing key expression for dictionary");
            break;
          }

          CJValueP key = keyExpr->exec(this);

          //---

          if (! isOperator(execData_->token(), CJOperator::Type::Colon)) {
            errorMsg(token, "Missing colon after key for dictionary");
            break;
          }

          execData_->next();

          CJExecExpressionP valueExpr = interpExpression();

          if (! valueExpr) {
            errorMsg(token, "Missing key expression for dictionary");
            break;
          }

          //---

          dict->addDictionaryValue(key, valueExpr);
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(dict));

        break;
      }

      //---

      if (opType == CJOperator::Type::Comma ||
          opType == CJOperator::Type::CloseRBracket ||
          opType == CJOperator::Type::CloseSBracket) {
        std::ostringstream ss; ss << *token;
        errorMsg(token, "Interp failed at operator: " + ss.str());
        break;
      }

      CJExecExpressionListP exprList = interpExpressionList();

      if (! exprList) {
        std::ostringstream ss; ss << *token;
        errorMsg(token, "Interp failed at token: " + ss.str());
        break;
      }

      execData_->addEToken(std::static_pointer_cast<CJToken>(exprList));
    }
    else if (type == CJToken::Type::Number || type == CJToken::Type::String ||
             type == CJToken::Type::Undefined || type == CJToken::Type::Null ||
             type == CJToken::Type::True || type == CJToken::Type::False) {
      CJExecExpressionListP exprList = interpExpressionList();

      if (! exprList) {
        std::ostringstream ss; ss << *token;
        errorMsg(token, "Interp failed at token: " + ss.str());
        break;
      }

      execData_->addEToken(std::static_pointer_cast<CJToken>(exprList));
    }
    else if (type == CJToken::Type::Keyword) {
      CJKeyword *keyword = token->cast<CJKeyword>();

      if      (keyword->type() == CJKeyword::Type::This) {
        CJExecThisP ethis = interpThis();

        execData_->addEToken(std::static_pointer_cast<CJToken>(ethis));
      }
      else if (keyword->type() == CJKeyword::Type::For) {
        execData_->next();

        CJExecForP efor = interpFor();

        if (! efor) {
          errorMsg(token, "Interp failed for 'for'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(efor));
      }
      else if (keyword->type() == CJKeyword::Type::If) {
        execData_->next();

        CJExecIfP eif = interpIf();

        if (! eif) {
          errorMsg(token, "Interp failed for 'if'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(eif));
      }
      else if (keyword->type() == CJKeyword::Type::While) {
        execData_->next();

        CJExecWhileP ewhile = interpWhile();

        if (! ewhile) {
          errorMsg(token, "Interp failed for 'while'");
          break;
        }

        execData_->addEToken(CJTokenP(ewhile));
      }
      else if (keyword->type() == CJKeyword::Type::Switch) {
        execData_->next();

        CJExecSwitchP eswitch = interpSwitch();

        if (! eswitch) {
          errorMsg(token, "Interp failed for 'switch'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(eswitch));
      }
      else if (keyword->type() == CJKeyword::Type::Var) {
        execData_->next();

        CJExecVarP evar = interpVar();

        if (! evar) {
          errorMsg(token, "Interp failed for 'var'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(evar));
      }
      else if (keyword->type() == CJKeyword::Type::With) {
        execData_->next();

        CJExecWithP ewith = interpWith();

        if (! ewith) {
          errorMsg(token, "Interp failed for 'with'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(ewith));
      }
      else if (keyword->type() == CJKeyword::Type::New) {
        execData_->next();

        CJExecNewP enew = interpNew();

        if (! enew) {
          errorMsg(token, "Interp failed for 'new'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(enew));
      }
      else if (keyword->type() == CJKeyword::Type::Delete) {
        execData_->next();

        CJExecDeleteP edelete = interpDelete();

        if (! edelete) {
          errorMsg(token, "Interp failed for 'delete'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(edelete));
      }
      else if (keyword->type() == CJKeyword::Type::Break) {
        execData_->next();

        CJExecBreakP ebreak = interpBreak();

        if (! ebreak) {
          errorMsg(token, "Interp failed for 'break'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(ebreak));
      }
      else if (keyword->type() == CJKeyword::Type::Continue) {
        execData_->next();

        CJExecContinueP econt = interpContinue();

        if (! econt) {
          errorMsg(token, "Interp failed for 'continue'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(econt));
      }
      else if (keyword->type() == CJKeyword::Type::Return) {
        execData_->next();

        CJExecReturnP eret = interpReturn();

        if (! eret) {
          errorMsg(token, "Interp failed for 'return'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(eret));
      }
      else if (keyword->type() == CJKeyword::Type::Function) {
        execData_->next();

        CJUserFunctionP efunction = interpUserFunction(/*named*/true);

        if (! efunction) {
          errorMsg(token, "Interp failed for 'function'");
          break;
        }

        CJDictionaryP scope;

        if (! userFunctions_.empty())
          scope = userFunctions_.back()->scope();
        else
          scope = currentScope();

        scope->setProperty(efunction->name(), std::static_pointer_cast<CJValue>(efunction));
      }
      else if (keyword->type() == CJKeyword::Type::Try) {
        execData_->next();

        CJExecTryP etry = interpTry();

        if (! etry) {
          errorMsg(token, "Interp failed for 'try'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(etry));
      }
      else if (keyword->type() == CJKeyword::Type::Throw) {
        execData_->next();

        CJExecThrowP ethrow = interpThrow();

        if (! ethrow) {
          errorMsg(token, "Interp failed for 'throw'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(ethrow));
      }
      else {
        errorMsg(token, "Interp failed at keyword: " + keyword->name());
        break;
      }
    }
    else {
      std::stringstream ss; ss << *token;
      errorMsg(token, "Interp failed at token: " + ss.str());
      break;
    }
  }

  execData_ = execDataStack_.back();

  execDataStack_.pop_back();
}

CJExecFunctionP
CJavaScript::
interpFunction()
{
  CJExecFunctionP func(new CJExecFunction);

  func->setLineNum(lineNum_);

  //---

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg(func, "Missing open round bracket");
    return CJExecFunctionP();
  }

  execData_->next();

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    CJExecExpressionListP exprList = interpExpressionList();

    if (! exprList) {
      return CJExecFunctionP();
    }

    func->setExprList(exprList);

    if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
      errorMsg(func, "Missing close round bracket");
      return CJExecFunctionP();
    }

    execData_->next();
  }
  else {
    execData_->next();
  }

  return func;
}

CJExecForP
CJavaScript::
interpFor()
{
  CJExecForP efor(new CJExecFor);

  efor->setLineNum(lineNum_);

  //---

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg(efor, "Missing open round bracket");
    return CJExecForP();
  }

  execData_->next();

  bool isIn = false;

  if (isExecKeyword(CJKeyword::Type::Var)) {
    execData_->next();

    if (isInterpForIn()) {
      CJExecIdentifiersP identifiers = interpIdentifiers();

      if (! identifiers) {
        errorMsg(efor, "Missing variable names for for in");
        return CJExecForP();
      }

      efor->setIdentifiers(identifiers, /*var*/true);

      isIn = true;
    }
    else {
      CJExecVarP evar = interpVar();

      if (! evar) {
        errorMsg(efor, "Interp failed for 'var'");
        return CJExecForP();
      }

      efor->setVar(evar);
    }
  }
  else {
    if (isInterpForIn()) {
      CJExecIdentifiersP identifiers = interpIdentifiers();

      if (! identifiers) {
        errorMsg(efor, "Missing variable names for for in");
        return CJExecForP();
      }

      efor->setIdentifiers(identifiers, /*var*/true);

      isIn = true;
    }
    else {
      CJExecExpressionListP exprList1 = interpExpressionList();

      if (! exprList1) {
        return CJExecForP();
      }

      efor->setExprList1(exprList1);
    }
  }

  if (isIn) {
    if (! isExecKeyword(CJKeyword::Type::In)) {
      errorMsg(efor, "Missing in for for in");
      return CJExecForP();
    }

    execData_->next();

    CJExecExpressionP expr = interpExpression();

    if (! expr) {
      return CJExecForP();
    }

    efor->setInExpr(expr);
  }
  else {
    CJExecExpressionListP exprList2 = interpExpressionList();

    if (! exprList2) {
      return CJExecForP();
    }

    efor->setExprList2(exprList2);

    CJExecExpressionListP exprList3 = interpExpressionList();

    if (! exprList3) {
      return CJExecForP();
    }

    efor->setExprList3(exprList3);
  }

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg(efor, "Missing close round bracket");
    return CJExecForP();
  }

  execData_->next();

  if (! isExecOperator(CJOperator::Type::OpenBrace)) {
    errorMsg(efor, "Missing open brace");
    return CJExecForP();
  }

  CJExecBlockP block = interpBlock(CJExecBlock::Type::Iterative);

  if (! block) {
    return CJExecForP();
  }

  efor->setBlock(block);

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

  if (! isExecKeyword(CJKeyword::Type::In)) {
    execData_->setPos(pos);
    return false;
  }

  execData_->setPos(pos);

  return true;
}

CJExecQuestionP
CJavaScript::
interpQuestion(CJExecExpressionP bexpr)
{
  CJExecQuestionP equestion(new CJExecQuestion(bexpr));

  equestion->setLineNum(lineNum_);

  //---

  CJExecExpressionP expr1 = interpExpression();

  if (! expr1) {
    return CJExecQuestionP();
  }

  equestion->setExpr1(expr1);

  if (! isExecOperator(CJOperator::Type::Colon)) {
    errorMsg(equestion, "Missing colon for ?:");
    return CJExecQuestionP();
  }

  execData_->next();

  CJExecExpressionP expr2 = interpExpression();

  if (! expr2) {
    return CJExecQuestionP();
  }

  equestion->setExpr2(expr2);

  if (isInterpDebug()) {
    std::cerr << "interpQuestion: " << *equestion << std::endl;
  }

  return equestion;
}

CJExecIfP
CJavaScript::
interpIf()
{
  CJExecIfP eif(new CJExecIf);

  eif->setLineNum(lineNum_);

  //---

  // parse if bracketed expression
  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg(eif, "Missing open bracket for if");
    return CJExecIfP();
  }

  execData_->next();

  CJExecExpressionListP exprList = interpExpressionList();

  if (! exprList) {
    return CJExecIfP();
  }

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg(eif, "Missing close bracket for if");
    return CJExecIfP();
  }

  execData_->next();

  //---

  // parse if block
  CJExecBlockP ifBlock = interpBlock(CJExecBlock::Type::Sequential);

  if (! ifBlock) {
    return CJExecIfP();
  }

  eif->setIfBlock(exprList, ifBlock);

  //---

  while (isExecKeyword(CJKeyword::Type::Else)) {
    execData_->next();

    if (isExecKeyword(CJKeyword::Type::If)) {
      execData_->next();

      // parse else if bracketed expression
      if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
        errorMsg(eif, "Missing open bracket for else if");
        return CJExecIfP();
      }

      execData_->next();

      CJExecExpressionListP exprList = interpExpressionList();

      if (! exprList) {
        return CJExecIfP();
      }

      if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
        errorMsg(eif, "Missing close bracket for else if");
        return CJExecIfP();
      }

      execData_->next();

      //---

      // parse else if block
      CJExecBlockP elseIfBlock = interpBlock(CJExecBlock::Type::Sequential);

      if (! elseIfBlock) {
        return CJExecIfP();
      }

      eif->addElseIfBlock(exprList, elseIfBlock);
    }
    else {
      // parse else block
      CJExecBlockP elseBlock = interpBlock(CJExecBlock::Type::Sequential);

      if (! elseBlock) {
        return CJExecIfP();
      }

      eif->setElseBlock(elseBlock);

      break;
    }
  }

  if (isInterpDebug()) {
    std::cerr << "interpIf: " << *eif << std::endl;
  }

  return eif;
}

CJExecWhileP
CJavaScript::
interpWhile()
{
  CJExecWhileP ewhile(new CJExecWhile);

  ewhile->setLineNum(lineNum_);

  //---

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg(ewhile, "Missing open bracket for while");
    return CJExecWhileP();
  }

  execData_->next();

  CJExecExpressionListP exprList = interpExpressionList();

  if (! exprList) {
    return CJExecWhileP();
  }

  ewhile->setExprList(exprList);

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg(ewhile, "Missing close bracket for while");
    return CJExecWhileP();
  }

  execData_->next();

  CJExecBlockP block = interpBlock(CJExecBlock::Type::Iterative);

  if (! block) {
    return CJExecWhileP();
  }

  ewhile->setBlock(block);

  if (isInterpDebug()) {
    std::cerr << "interpWhile: " << *ewhile << std::endl;
  }

  return ewhile;
}

CJExecSwitchP
CJavaScript::
interpSwitch()
{
  CJExecSwitchP eswitch(new CJExecSwitch);

  eswitch->setLineNum(lineNum_);

  //---

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg(eswitch, "Missing open bracket for switch");
    return CJExecSwitchP();
  }

  execData_->next();

  CJExecExpressionListP exprList = interpExpressionList();

  if (! exprList) {
    return CJExecSwitchP();
  }

  eswitch->setExprList(exprList);

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg(eswitch, "Missing close bracket for switch");
    return CJExecSwitchP();
  }

  execData_->next();

  //---

  if (! isExecOperator(CJOperator::Type::OpenBrace)) {
    errorMsg(eswitch, "Missing open brace");
    return CJExecSwitchP();
  }

  execData_->next();

  CJExecBlockP      caseBlock;
  CJExecExpressionP caseExpr;
  CJExecBlockP      defBlock;

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

            eswitch->addCase(CJExecExpressionP(caseExpr), caseBlock);

            caseExpr  = CJExecExpressionP();
            caseBlock = CJExecBlockP();
          }
          else if (defBlock) {
            defBlock->interp(this);

            eswitch->setDefault(defBlock);

            defBlock = CJExecBlockP();
          }

          if (keyword->type() == CJKeyword::Type::Case) {
            caseExpr = interpExpression();

            if (! caseExpr) {
              errorMsg(eswitch, "Invalid case value");
              return CJExecSwitchP();
            }
          }

          if (! isExecOperator(CJOperator::Type::Colon)) {
            errorMsg(eswitch, "Missing colon for case expression");
            return CJExecSwitchP();
          }

          execData_->next();

          if (keyword->type() == CJKeyword::Type::Case) {
            caseBlock = CJExecBlockP(new CJExecBlock(CJExecBlock::Type::Sequential));

            caseBlock->setLineNum(lineNum_);
          }
          else {
            defBlock = CJExecBlockP(new CJExecBlock(CJExecBlock::Type::Sequential));

            defBlock->setLineNum(lineNum_);
          }

          continue;
        }
        else if (keyword->type() == CJKeyword::Type::Break) {
          execData_->next();

          if      (caseExpr && caseBlock) {
            caseBlock->interp(this);

            eswitch->addCase(caseExpr, caseBlock);

            caseExpr  = CJExecExpressionP();
            caseBlock = CJExecBlockP();
          }
          else if (defBlock) {
            defBlock->interp(this);

            eswitch->setDefault(defBlock);

            defBlock = CJExecBlockP();
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
      errorMsg(eswitch, "Missing colon for case expression");
      return CJExecSwitchP();
    }

    execData_->next();
  }

  if (! isExecOperator(CJOperator::Type::CloseBrace)) {
    errorMsg(eswitch, "Missing close brace");
    return CJExecSwitchP();
  }

  execData_->next();

  //---

  if      (caseExpr && caseBlock) {
    caseBlock->interp(this);

    eswitch->addCase(caseExpr, caseBlock);

    caseExpr  = CJExecExpressionP();
    caseBlock = CJExecBlockP();
  }
  else if (defBlock) {
    defBlock->interp(this);

    eswitch->setDefault(defBlock);

    defBlock = CJExecBlockP();
  }

  //---

  if (isInterpDebug()) {
    std::cerr << "interpSwitch: " << *eswitch << std::endl;
  }

  return eswitch;
}

CJExecVarP
CJavaScript::
interpVar()
{
  CJExecVarP var(new CJExecVar);

  var->setLineNum(lineNum_);

  //---

  while (! execData_->eof()) {
    CJTokenP token = execData_->token();

    // variable name
    if (token->type() != CJToken::Type::Identifier) {
      errorMsg(var, "Missing identifier");
      return CJExecVarP();
    }

    CJExecIdentifiersP identifiers = interpIdentifiers();

    // variable init value
    if (isExecOperator(CJOperator::Type::Assign)) {
      execData_->next();

      if      (isExecOperator(CJOperator::Type::OpenSBracket)) {
        CJExecArrayP array = interpArray();

        if (! array) {
          return CJExecVarP();
        }

        var->addVarValue(identifiers, array);
      }
      else if (isExecOperator(CJOperator::Type::OpenBrace)) {
        if (isInterpDictionary()) {
          CJExecDictionaryP dict = interpDictionary();

          if (! dict) {
            return CJExecVarP();
          }

          var->addVarValue(identifiers, dict);
        }
        else {
          CJExecBlockP block = interpBlock(CJExecBlock::Type::Sequential);

          if (! block) {
            return CJExecVarP();
          }

          var->addVarValue(identifiers, block);
        }
      }
      else {
        CJExecExpressionP expr = interpExpression();

        if (! expr) {
          return CJExecVarP();
        }

        var->addVarValue(identifiers, expr);
      }
    }
    else {
      var->addVarValue(identifiers, CJExecExpressionP());
    }

    //---

    if (! isExecOperator(CJOperator::Type::Comma)) {
      break;
    }

    execData_->next();
  }

  if (isExecOperator(CJOperator::Type::SemiColon)) {
    execData_->next();
  }

  if (isInterpDebug()) {
    std::cerr << "interpVar: " << *var << std::endl;
  }

  return var;
}

CJExecWithP
CJavaScript::
interpWith()
{
  CJExecWithP ewith(new CJExecWith);

  ewith->setLineNum(lineNum_);

  //---

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg(ewith, "Missing open bracket for with");
    return CJExecWithP();
  }

  execData_->next();

  CJExecIdentifiersP identifiers = interpIdentifiers();

  if (! identifiers) {
    return CJExecWithP();
  }

  ewith->setIdentifiers(identifiers);

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg(ewith, "Missing close bracket for with");
    return CJExecWithP();
  }

  execData_->next();

  CJExecBlockP block = interpBlock(CJExecBlock::Type::Iterative);

  if (! block) {
    return CJExecWithP();
  }

  ewith->setBlock(block);

  if (isInterpDebug()) {
    std::cerr << "interpWith: " << *ewith << std::endl;
  }

  return ewith;
}

CJExecArrayP
CJavaScript::
interpArray()
{
  CJExecArrayP array(new CJExecArray);

  array->setLineNum(lineNum_);

  //---

  if (! isExecOperator(CJOperator::Type::OpenSBracket))
    return CJExecArrayP();

  execData_->next();

  while (! isExecOperator(CJOperator::Type::CloseSBracket)) {
    CJExecExpressionP expr = interpExpression();

    if (! expr)
      return CJExecArrayP();

    array->addValue(expr);

    if (! isExecOperator(CJOperator::Type::Comma))
      break;

    execData_->next();
  }

  if (! isExecOperator(CJOperator::Type::CloseSBracket))
    return CJExecArrayP();

  execData_->next();

  if (isInterpDebug()) {
    std::cerr << "interpExecArray: " << *array << std::endl;
  }

  return array;
}

bool
CJavaScript::
isInterpDictionary() const
{
  int pos = execData_->pos();

  if (! isExecOperator(CJOperator::Type::OpenBrace))
    return false;

  execData_->next();

  if (isExecOperator(CJOperator::Type::CloseBrace)) {
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

  if (! isExecOperator(CJOperator::Type::Colon)) {
    execData_->setPos(pos);
    return false;
  }

  execData_->setPos(pos);

  return true;
}

CJExecDictionaryP
CJavaScript::
interpDictionary()
{
  CJExecDictionaryP dict(new CJExecDictionary);

  dict->setLineNum(lineNum_);

  //---

  if (! isExecOperator(CJOperator::Type::OpenBrace))
    return CJExecDictionaryP();

  execData_->next();

  if (! isExecOperator(CJOperator::Type::CloseBrace)) {
    while (! execData_->eof()) {
      CJValueP key;

      CJTokenP token = execData_->token();

      if      (token->type() == CJToken::Type::Identifier)
        key = createStringValue(token->cast<CJIdentifier>()->name());
      else if (token->type() == CJToken::Type::Number)
        key = std::static_pointer_cast<CJValue>(token);
      else {
        errorMsg(dict, "Invalid key value type");
        return CJExecDictionaryP();
      }

      execData_->next();

      if (! isExecOperator(CJOperator::Type::Colon)) {
        errorMsg(dict, "Missing colon for key value");
        return CJExecDictionaryP();
      }

      execData_->next();

      CJExecExpressionP expr = interpExpression();

      if (! expr) {
        errorMsg(dict, "Invalid key value");
        return CJExecDictionaryP();
      }

      dict->addDictionaryValue(key, expr);

      if (! isExecOperator(CJOperator::Type::Comma))
        break;

      execData_->next();
    }

    if (! isExecOperator(CJOperator::Type::CloseBrace)) {
      errorMsg(dict, "Missing close brace for dictionary");
      return CJExecDictionaryP();
    }

     execData_->next();
  }
  else {
    execData_->next();
  }

  return dict;
}

CJExecNewP
CJavaScript::
interpNew()
{
  CJExecNewP enew = CJExecNewP(new CJExecNew);

  enew->setLineNum(lineNum_);

  //---

  // get object name
  CJTokenP token = execData_->token();

  if (token->type() != CJToken::Type::Identifier) {
    errorMsg(enew, "Missing identifier");
    return CJExecNewP();
  }

  CJIdentifier *identifier = token->cast<CJIdentifier>();

  std::string typeName = identifier->name();

  enew->setTypeName(typeName);

  execData_->next();

  // get args (optional)
  if (isExecOperator(CJOperator::Type::OpenRBracket)) {
    execData_->next();

    if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
      CJExecExpressionListP exprList = interpExpressionList();

      enew->setExprList(exprList);
    }

    if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
      errorMsg(enew, "Missing close bracket for new");
      return CJExecNewP();
    }

    execData_->next();
  }

  //---

  if (isInterpDebug())
    std::cerr << "interpNew: " << *enew << std::endl;

  return enew;
}

CJExecDeleteP
CJavaScript::
interpDelete()
{
  CJExecDeleteP edelete(new CJExecDelete);

  edelete->setLineNum(lineNum_);

  //---

  CJExecIdentifiersP identifiers = interpIdentifiers();

  if (! identifiers)
    return CJExecDeleteP();

  edelete->setIdentifiers(identifiers);

  if (isInterpDebug()) {
    std::cerr << "interpDelete: " << *edelete << std::endl;
  }

  return edelete;
}

CJExecBreakP
CJavaScript::
interpBreak()
{
  CJExecBreakP ebreak(new CJExecBreak);

  ebreak->setLineNum(lineNum_);

  return ebreak;
}

CJExecContinueP
CJavaScript::
interpContinue()
{
  CJExecContinueP econt(new CJExecContinue);

  econt->setLineNum(lineNum_);

  return econt;
}

CJExecReturnP
CJavaScript::
interpReturn()
{
  CJExecReturnP eret(new CJExecReturn);

  eret->setLineNum(lineNum_);

  //---

  CJExecExpressionP expr = interpExpression();

  if (! expr)
    return CJExecReturnP();

  eret->setExpr(expr);

  if (isInterpDebug()) {
    std::cerr << "interpReturn: " << *eret << std::endl;
  }

  return eret;
}

CJExecTryP
CJavaScript::
interpTry()
{
  CJExecTryP etry(new CJExecTry);

  etry->setLineNum(lineNum_);

  //---

  CJExecBlockP block1 = interpBlock(CJExecBlock::Type::Sequential);

  if (! block1) {
    return CJExecTryP();
  }

  etry->setTryBlock(block1);

  if (! isExecKeyword(CJKeyword::Type::Catch)) {
    errorMsg(etry, "Missing catch for trye");
    return CJExecTryP();
  }

  execData_->next();

  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg(etry, "Missing open bracket for catch");
    return CJExecTryP();
  }

  execData_->next();

  CJExecIdentifiersP identifiers = interpIdentifiers();

  if (! identifiers) {
    return CJExecTryP();
  }

  etry->setCatchIdentifiers(identifiers);

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg(etry, "Missing close bracket for catch");
    return CJExecTryP();
  }

  execData_->next();

  CJExecBlockP block2 = interpBlock(CJExecBlock::Type::Sequential);

  if (! block2) {
    return CJExecTryP();
  }

  etry->setCatchBlock(block2);

  if (isInterpDebug()) {
    std::cerr << "interpTry: " << *etry << std::endl;
  }

  return etry;
}

CJExecThrowP
CJavaScript::
interpThrow()
{
  CJExecThrowP ethrow(new CJExecThrow);

  ethrow->setLineNum(lineNum_);

  //---

  CJExecExpressionP expr = interpExpression();

  if (! expr) {
    return CJExecThrowP();
  }

  ethrow->setExpression(expr);

  return ethrow;
}

CJUserFunctionP
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
      errorMsg(token, "Missing identifier");
      return CJUserFunctionP();
    }
  }

  CJUserFunctionP userFn(new CJUserFunction(this, name));

  //---

  // arguments
  if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
    errorMsg(token, "Missing open bracket for function");
    return CJUserFunctionP();
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
      errorMsg(token, "Missing comma for function args");
      return CJUserFunctionP();
    }

    execData_->next();

    token = execData_->token();

    if (token->type() != CJToken::Type::Identifier) {
      errorMsg(token, "Missing arg after comma for function args");
      return CJUserFunctionP();
    }
  }

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    errorMsg(token, "Missing close bracket for function");
    return CJUserFunctionP();
  }

  execData_->next();

  userFn->setArgs(args);

  //---

//printUserFunctions("interpUserFunction");
  userFn->setScope(this, userFunctions());

  pushUserFunction(userFn);

  // function block
  CJExecBlockP block = interpBlock(CJExecBlock::Type::Function);

  if (block)
    userFn->setBlock(block);

  popUserFunction();

  if (! block)
    return CJUserFunctionP();

  //---

  if (isInterpDebug()) {
    std::cerr << "interpUserFunction: " << *userFn << std::endl;
  }

  return userFn;
}

CJExecExpressionListP
CJavaScript::
interpExpressionList()
{
  CJExecExpressionListP exprList(new CJExecExpressionList);

  exprList->setLineNum(lineNum_);

  //---

  CJExecExpressionList::Expressions expressions;

  while (! execData_->eof()) {
    CJExecExpressionP expr = interpExpression();

    if (! expr)
      return CJExecExpressionListP();

    expressions.push_back(expr);

    if (! isExecOperator(CJOperator::Type::Comma))
      break;

    execData_->next();
  }

  //---

  for (const auto &e : expressions)
    exprList->addExpression(e);

  //---

  if (isExecOperator(CJOperator::Type::SemiColon))
    execData_->next();

  if (isInterpDebug()) {
    std::cerr << "interpExpressionList: " << *exprList << std::endl;
  }

  return exprList;
}

CJExecExpressionP
CJavaScript::
interpExpression()
{
  CJExecExpressionP expr(new CJExecExpression);

  expr->setLineNum(lineNum_);

  while (! execData_->eof()) {
    CJTokenP token = execData_->token();

    if      (token->type() == CJToken::Type::Identifier) {
      CJExecIndexExpressionP iexpr;

      CJExecIdentifiersP identifiers = interpIdentifiers();

      while (isExecOperator(CJOperator::Type::OpenSBracket)) {
        execData_->next();

        CJExecIndexExpressionP iexpr1(new CJExecIndexExpression);

        iexpr1->setLineNum(lineNum_);

        if (identifiers) {
          iexpr1->setIdentifiers(identifiers);

          identifiers = 0;
        }
        else {
          iexpr1->setExpression(iexpr);
        }

        iexpr = iexpr1;

        //----

        CJExecExpressionP expr1 = interpExpression();

        if (expr1)
          iexpr->setIndexExpression(expr1);

        if (! isExecOperator(CJOperator::Type::CloseSBracket)) {
          errorMsg(expr, "Missing close square bracket");
          return CJExecExpressionP();
        }

        execData_->next();
      }

      if      (isExecOperator(CJOperator::Type::OpenRBracket)) {
        CJExecFunctionP function = interpFunction();

        if (! function) {
          return CJExecExpressionP();
        }

        if (iexpr) {
          if (isInterpDebug()) {
            std::cerr << "interpIndexExpression: " << *iexpr << std::endl;
          }

          function->setExpression(iexpr);
        }
        else {
          function->setIdentifiers(identifiers);
        }

        if (isInterpDebug()) {
          std::cerr << "interpFunction: " << *function << std::endl;
        }

        expr->addToken(CJTokenP(function));

#if 0
        while (isExecOperator(CJOperator::Type::Scope)) {
          execData_->next();

          CJExecIdentifiersP identifiers = interpIdentifiers();

          if (! identifiers) {
            return CJExecExpressionP();
          }

          CJExecFunctionP function1 = interpFunction();

          if (! function1) {
            return CJExecExpressionP();
          }

          function1->setIdentifiers(identifiers);

          if (isInterpDebug()) {
            std::cerr << "interpFunction: " << *function1 << std::endl;
          }

          //---

          function->addFunction(function1);
        }
#endif
      }
      else {
        if (isExecOperator(CJOperator::Type::Increment) ||
            isExecOperator(CJOperator::Type::Decrement)) {
          CJTokenP token1 = execData_->token();

          execData_->next();

          CJExecIncrDecrExpressionP incrDecr(new CJExecIncrDecrExpression);

          incrDecr->setLineNum(lineNum_);

          CJOperatorP op1 = std::static_pointer_cast<CJOperator>(token1);

          if (iexpr) {
            if (isInterpDebug()) {
              std::cerr << "interpIndexExpression: " << *iexpr << std::endl;
            }

            incrDecr->setExpression(iexpr);
          }
          else {
            incrDecr->setIdentifiers(identifiers);
          }

          incrDecr->setOperator(op1);
          incrDecr->setPost    (true);

          expr->addToken(std::static_pointer_cast<CJToken>(incrDecr));
        }
        else {
          // add index expression or identifiers
          if (iexpr) {
            if (isInterpDebug()) {
              std::cerr << "interpIndexExpression: " << *iexpr << std::endl;
            }

            expr->addToken(std::static_pointer_cast<CJToken>(iexpr));
          }
          else {
            expr->addToken(std::static_pointer_cast<CJToken>(identifiers));
          }
        }
      }
    }
    else if (token->isValue()) {
      execData_->next();

      if (token->isProtoValue() && isExecOperator(CJOperator::Type::Scope)) {
        execData_->next();

        CJExecIdentifiersP identifiers = interpIdentifiers();

        CJExecFunctionP function = interpFunction();

        if (! function) {
          return CJExecExpressionP();
        }

        expr->addToken(token);

        function->setExpression (expr);
        function->setIdentifiers(identifiers);

        if (isInterpDebug()) {
          std::cerr << "interpFunction: " << *function << std::endl;
        }

        //---

        expr = CJExecExpressionP(new CJExecExpression);

        expr->setLineNum(lineNum_);

        expr->addToken(CJTokenP(function));
      }
      else {
        expr->addToken(token);
      }
    }
    else if (token->type() == CJToken::Type::Operator) {
      CJOperatorP op = std::static_pointer_cast<CJOperator>(token);

      CJOperator::Type opType = op->type();

      if      (op->isAssign()) {
        execData_->next();

        CJExecAssignExpressionP assign(new CJExecAssignExpression);

        assign->setLineNum(lineNum_);

        assign->setLExpression(expr);
        assign->setOperator   (op);

        CJExecExpressionP expr1 = interpExpression();

        if (! expr1) {
          errorMsg(expr, "invalid assignment rhs expression");
          return CJExecExpressionP();
        }

        assign->setRExpression(expr1);

        return std::static_pointer_cast<CJExecExpression>(assign);
      }
      else if (opType == CJOperator::Type::Increment ||
               opType == CJOperator::Type::Decrement) {
        execData_->next();

        CJTokenP token1 = execData_->token();

        if      (token1->type() == CJToken::Type::Identifier) {
          CJExecIdentifiersP identifiers = interpIdentifiers();

          CJExecIncrDecrExpressionP incrDecr(new CJExecIncrDecrExpression);

          incrDecr->setLineNum(lineNum_);

          incrDecr->setIdentifiers(identifiers);
          incrDecr->setOperator   (op);
          incrDecr->setPost       (false);

          expr->addToken(std::static_pointer_cast<CJToken>(incrDecr));
        }
        else {
          errorMsg(expr, "invalid increment/decrement rhs");
          return CJExecExpressionP();
        }
      }
      else if (opType == CJOperator::Type::Question) {
        execData_->next();

        CJExecQuestionP equestion = interpQuestion(expr);

        if (! equestion) {
          errorMsg(expr, "Interp failed for '?'");
          break;
        }

        expr = CJExecExpressionP(new CJExecExpression);

        expr->setLineNum(lineNum_);

        expr->addToken(std::static_pointer_cast<CJToken>(equestion));
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

        CJExecExpressionP expr1 = interpExpression();

        if (expr1)
          expr->addToken(std::static_pointer_cast<CJToken>(expr1));

        if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
          errorMsg(expr, "Missing close round bracket");
          return CJExecExpressionP();
        }

        execData_->next();
      }
      else if (opType == CJOperator::Type::CloseRBracket) {
        return expr;
      }
      else if (opType == CJOperator::Type::OpenSBracket) {
        if (! expr->hasTokens()) {
          CJExecArrayP array = interpArray();

          if (! array) {
            return CJExecExpressionP();
          }

          expr->addToken(CJTokenP(array));
        }
        else {
          while (isExecOperator(CJOperator::Type::OpenSBracket)) {
            CJExecIndexExpressionP iexpr1 = interpIndexExpression();

            expr->addToken(CJTokenP(iexpr1));

            execData_->next();
          }
        }
#if 0
        if (isExecOperator(CJOperator::Type::Scope)) {
          execData_->next();

          CJExecIdentifiersP identifiers = interpIdentifiers();

          CJExecFunctionP function = interpFunction();

          if (! function) {
            return CJExecExpressionP();
          }

          expr->addToken(CJTokenP(array));

          function->setExpression (expr);
          function->setIdentifiers(identifiers);

          if (isInterpDebug()) {
            std::cerr << "interpFunction: " << *function << std::endl;
          }

          //---

          expr = CJExecExpressionP(new CJExecExpression);

          expr->setLineNum(lineNum_);

          expr->addToken(CJTokenP(function));
        }
        else {
          expr->addToken(CJTokenP(array));
        }
#endif
      }
      else if (opType == CJOperator::Type::CloseSBracket) {
        return expr;
      }
      else if (opType == CJOperator::Type::OpenBrace) {
        if (isInterpDictionary()) {
          CJExecDictionaryP dict = interpDictionary();

          if (! dict) {
            return CJExecExpressionP();
          }

#if 0
          if (isExecOperator(CJOperator::Type::Scope)) {
            execData_->next();

            CJExecIdentifiersP identifiers = interpIdentifiers();

            CJExecFunctionP function = interpFunction();

            if (! function) {
              return CJExecExpressionP();
            }

            expr->addToken(CJTokenP(dict));

            function->setExpression (expr);
            function->setIdentifiers(identifiers);

            if (isInterpDebug()) {
              std::cerr << "interpFunction: " << *function << std::endl;
            }

            //---

            expr = CJExecExpressionP(new CJExecExpression);

            expr->setLineNum(lineNum_);

            expr->addToken(CJTokenP(function));
          }
          else {
            expr->addToken(CJTokenP(dict));
          }
#else
          expr->addToken(CJTokenP(dict));
#endif
        }
        else {
          CJExecBlockP block = interpBlock(CJExecBlock::Type::Iterative);

          if (! block) {
            return CJExecExpressionP();
          }

          expr->addToken(block);
        }
      }
      else if (opType == CJOperator::Type::CloseBrace) {
        return expr;
      }
      else if (opType == CJOperator::Type::Scope) {
        execData_->next();

        expr->addToken(token);
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

        CJExecNewP enew = interpNew();

        if (! enew) {
          errorMsg(expr, "Interp failed for 'new'");
          break;
        }

        expr->addToken(std::static_pointer_cast<CJToken>(enew));
      }
      else if (keyword->type() == CJKeyword::Type::Function) {
        execData_->next();

        CJUserFunctionP efunction = interpUserFunction(/*named*/false);

        if (! efunction) {
          errorMsg(expr, "Interp failed for 'function'");
          break;
        }

        CJValueP value(efunction);

        setProperty(efunction->name(), value);

        expr->addToken(value);
      }
      else {
        errorMsg(expr, "Interp failed at keyword: " + keyword->name());
        break;
      }
    }
    else {
      std::stringstream ss; ss << *token;
      errorMsg(expr, "Interp failed at token: " + ss.str());
      return CJExecExpressionP();
    }
  }

  if (isInterpDebug()) {
    std::cerr << "interpExpression: " << *expr << std::endl;
  }

  return expr;
}

CJExecThisP
CJavaScript::
interpThis()
{
  if (! isExecKeyword(CJKeyword::Type::This)) {
    errorMsg(execData_->token(), "Missing keyword this");
    return CJExecThisP();
  }

  execData_->next();

  CJExecThisP ethis(new CJExecThis);

  if      (isExecOperator(CJOperator::Type::Scope)) {
    execData_->next();

    CJExecIdentifiersP identifiers = interpIdentifiers();

    ethis->setIdentifiers(identifiers);
  }
  else if (isExecOperator(CJOperator::Type::OpenSBracket)) {
    CJExecIndexExpressionP iexpr = interpIndexExpression();

    ethis->setIndexExpression(iexpr);
  }

  if (isExecOperator(CJOperator::Type::Assign)) {
    execData_->next();

    CJExecExpressionP expr = interpExpression();

    if (! expr) {
      errorMsg(execData_->token(), "Missing expression for assign");
      return CJExecThisP();
    }

    ethis->setAssignExpression(expr);
  }

  return ethis;
}

CJExecIndexExpressionP
CJavaScript::
interpIndexExpression()
{
  if (! isExecOperator(CJOperator::Type::OpenSBracket)) {
    errorMsg(execData_->token(), "Missing open square bracket");
    return CJExecIndexExpressionP();
  }

  execData_->next();

  CJExecIndexExpressionP iexpr(new CJExecIndexExpression);

  iexpr->setLineNum(lineNum_);

  CJExecExpressionP expr = interpExpression();

  if (expr)
    iexpr->setIndexExpression(expr);

  if (! isExecOperator(CJOperator::Type::CloseSBracket)) {
    errorMsg(execData_->token(), "Missing close square bracket");
    return CJExecIndexExpressionP();
  }

  execData_->next();

  if (isInterpDebug()) {
    std::cerr << "interpIndexExpression: " << *iexpr << std::endl;
  }

  return iexpr;
}

CJExecIdentifiersP
CJavaScript::
interpIdentifiers()
{
  CJExecIdentifiersP identifiers(new CJExecIdentifiers);

  identifiers->setLineNum(lineNum_);

  //---

  CJTokenP token = execData_->token();

  if (token->type() != CJToken::Type::Identifier)
    return CJExecIdentifiersP();

  execData_->next();

  identifiers->addIdentifier(token->cast<CJIdentifier>());

  while (isExecOperator(CJOperator::Type::Scope)) {
    execData_->next();

    CJTokenP token = execData_->token();

    if (token->type() != CJToken::Type::Identifier) {
      return CJExecIdentifiersP();
    }

    execData_->next();

    identifiers->addIdentifier(token->cast<CJIdentifier>());
  }

  if (isInterpDebug()) {
    std::cerr << "interpIdentifiers: " << *identifiers << std::endl;
  }

  return identifiers;
}

CJExecBlockP
CJavaScript::
interpBlock(CJExecBlock::Type type)
{
  CJExecBlockP block(new CJExecBlock(type));

  block->setLineNum(lineNum_);

  //---

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
      errorMsg(block, "Missing close brace");
      return CJExecBlockP();
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
isExecKeyword(CJKeyword::Type type) const
{
  if (execData_->eof())
    return false;

  CJTokenP token = execData_->token();

  return isKeyword(token, type);
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
  if      (value1->type() == CJValue::Type::String && value2->type() == CJValue::Type::String) {
    std::string s1 = value1->toString();
    std::string s2 = value2->toString();

    if (s1 < s2) return -1;
    if (s1 > s2) return  1;

    return 0;
  }
  else if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
    double r1 = value1->toReal();
    double r2 = value2->toReal();

    if (r1 < r2) return -1;
    if (r1 > r2) return  1;

    return 0;
  }
  else if (value1->type() == CJValue::Type::Undefined ||
           value2->type() == CJValue::Type::Undefined) {
    if (value1->type() == value2->type())
      return 0;

    if (value1->type() == CJValue::Type::Null || value2->type() == CJValue::Type::Null)
      return 0;

    return -1; // special value for always false
  }
  else if (value1->type() == CJValue::Type::Null ||
           value2->type() == CJValue::Type::Null) {
    if (value1->type() == value2->type())
      return 0;

    if (value1->type() == CJValue::Type::Undefined || value2->type() == CJValue::Type::Undefined)
      return 0;

    return -1; // special value for always false
  }
  else if (value1->isBoolType() || value2->isBoolType()) {
    bool b1 = value1->toBoolean();
    bool b2 = value2->toBoolean();

    if (b1 < b2) return -1;
    if (b1 > b2) return  1;

    return 0;
  }
  else if (value1->type() == CJValue::Type::Number || value2->type() == CJValue::Type::Number) {
    double r1 = value1->toReal();
    double r2 = value2->toReal();

    if (r1 < r2) return -1;
    if (r1 > r2) return  1;

    return 0;
  }
  else if (value1->type() == CJValue::Type::String || value2->type() == CJValue::Type::String) {
    std::string s1 = value1->toString();
    std::string s2 = value2->toString();

    if (s1 < s2) return -1;
    if (s1 > s2) return  1;

    return 0;
  }
  else if (value1->type() == CJValue::Type::Object && value2->type() == CJValue::Type::Object) {
    CJObjP obj1 = std::static_pointer_cast<CJObj>(value1);
    CJObjP obj2 = std::static_pointer_cast<CJObj>(value2);

    return obj1->cmp(obj2);
  }
  else {
    double r1 = value1->toReal();
    double r2 = value2->toReal();

    if (r1 < r2) return -1;
    if (r1 > r2) return  1;

    return 0;
  }
}

CJValueP
CJavaScript::
execBinaryOp(CJOperator::Type op, CJValueP value1, CJValueP value2)
{
  if (! value1 || ! value2) {
    if      (op == CJOperator::Type::StrictEqual) {
      return createBoolValue(value1 == value2);
    }
    else if (op == CJOperator::Type::StrictNotEqual) {
      return createBoolValue(value1 != value2);
    }
    else {
      return CJValueP();
    }
  }

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
        double r1 = value1->toReal();
        double r2 = value2->toReal();

        return CJValueP(createNumberValue(r1 + r2));
      }
      else if (value1->type() == CJValue::Type::Object ||
               value2->type() == CJValue::Type::Object) {
        std::string s1 = value1->toString();
        std::string s2 = value2->toString();

        return createStringValue(s1 + s2);
      }
      else if (value1->type() == CJValue::Type::Number ||
               value2->type() == CJValue::Type::Number) {
        double r1 = value1->toReal();
        double r2 = value2->toReal();

        return CJValueP(createNumberValue(r1 + r2));
      }
      else {
        std::string s1 = value1->toString();
        std::string s2 = value2->toString();

        return createStringValue(s1 + s2);
      }

      break;
    case CJOperator::Type::Minus:
      if (value1->type() == CJValue::Type::Number ||
          value2->type() == CJValue::Type::Number) {
        double r1 = value1->toReal();
        double r2 = value2->toReal();

        return CJValueP(createNumberValue(r1 - r2));
      }

      break;
    case CJOperator::Type::Times:
      if (value1->type() == CJValue::Type::Number ||
          value2->type() == CJValue::Type::Number) {
        double r1 = value1->toReal();
        double r2 = value2->toReal();

        return CJValueP(createNumberValue(r1 * r2));
      }

      break;
    case CJOperator::Type::Divide:
      if (value1->type() == CJValue::Type::Number ||
          value2->type() == CJValue::Type::Number) {
        double r1 = value1->toReal();
        double r2 = value2->toReal();

        if (r2 != 0)
          return CJValueP(createNumberValue(r1 / r2));
        else
          return CJValueP(createNumberValue(CJUtil::getNaN()));
      }

      break;
    case CJOperator::Type::Modulus:
      if (value1->type() == CJValue::Type::Number ||
          value2->type() == CJValue::Type::Number) {
        double r1 = value1->toReal();
        double r2 = value2->toReal();

        return CJValueP(createNumberValue(CJUtil::realModulus(r1, r2)));
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

    case CJOperator::Type::LeftShift: {
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        long res = (i1 << i2);

        return CJValueP(createNumberValue(res));
      }

      break;
    }
    case CJOperator::Type::RightShift: {
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        long res = (i1 >> i2);

        return CJValueP(createNumberValue(res));
      }

      break;
    }
    case CJOperator::Type::UnsignedRightShift: {
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        ulong i1 = value1->toInteger();
        long  i2 = value2->toInteger();

        long res = (i1 >> i2);

        return CJValueP(createNumberValue(res));
      }

      break;
    }

    case CJOperator::Type::InstanceOf: {
      CJObjTypeP valueType1, valueType2;

      if      (value1->type() == CJToken::Type::Object)
        valueType1 = value1->cast<CJObj>()->valueType();
      else if (value1->type() == CJToken::Type::Function) {
        CJFunctionP fn = std::static_pointer_cast<CJFunction>(value1);

        if (fn->type() == CJFunction::Type::Type) {
          CJTypeFunctionP typeFn = std::static_pointer_cast<CJTypeFunction>(value1);

          valueType1 = typeFn->objectType();
        }
      }
      else
        valueType1 = value1->valueType();

      if      (value2->type() == CJToken::Type::Object)
        valueType2 = value2->cast<CJObj>()->valueType();
      else if (value2->type() == CJToken::Type::Function) {
        CJFunctionP fn = std::static_pointer_cast<CJFunction>(value2);

        if (fn->type() == CJFunction::Type::Type) {
          CJTypeFunctionP typeFn = std::static_pointer_cast<CJTypeFunction>(value2);

          valueType2 = typeFn->objectType();
        }
      }
      else
        valueType2 = value2->valueType();

      return CJValueP(createBoolValue(valueType1.get() == valueType2.get()));
    }

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
  if (! value) {
    if      (op == CJOperator::Type::TypeOf)
      return CJValueP(createStringValue("undefined"));
    else if (op == CJOperator::Type::LogicalNot)
      return CJValueP(createBoolValue(true));
    else
      return CJValueP();
  }

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
      if (value->type() == CJValue::Type::Object)
        return false;

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

    case CJOperator::Type::TypeOf: {
      std::string res = value->valueType()->name();

      return CJValueP(createStringValue(res));
    }

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

  CJTokenP token;

  if      (name == "null")
    token = CJTokenP(createNullValue());
  else if (name == "true")
    token = CJTokenP(createTrueValue());
  else if (name == "false")
    token = CJTokenP(createFalseValue());
  else {
    auto k = isKeyword(name);

    if (k.first)
      token = CJTokenP(new CJKeyword(k.second));
    else
      token = CJTokenP(new CJIdentifier(name));
  }

  token->setLineNum(lineNum_);

  tokens_.push_back(token);
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

        hexValue = 16*hexValue + CJUtil::hexCharValue(c);
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
errorMsg(CJTokenP token, const std::string &msg) const
{
  errorMsg(token.get(), msg);
}

void
CJavaScript::
errorMsg(CJToken *token, const std::string &msg) const
{
  if (fileName_ != "")
    std::cerr << fileName_ << "@" << token->lineNum() << ": ";
  else
    std::cerr << token->lineNum() << ": ";

  std::cerr << msg << std::endl;
}

void
CJavaScript::
errorMsg(const std::string &msg) const
{
  if (fileName_ != "")
    std::cerr << fileName_ << "@" << lineNum_ << ": ";

  std::cerr << msg << std::endl;
}

//------

CJValueP
CJRandFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() != 0) {
    js->errorMsg("Wrong number of arguments values");
    return CJValueP();
  }

  double res = (1.0*rand())/RAND_MAX;

  return js->createNumberValue(res);
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

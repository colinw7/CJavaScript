#include <CJavaScript.h>
#include <CJGlobalFunction.h>
#include <CJFunctionFunction.h>
#include <CJMath.h>
#include <CJJSON.h>
#include <CJSystem.h>
#include <CJDocument.h>
#include <CJConsole.h>
#include <CJDebug.h>
#include <CJError.h>
#include <CJUtil.h>
#include <CJGetterSetter.h>
#include <CJTokenValue.h>

#include <CJArrayFunction.h>
#include <CJBooleanFunction.h>
#include <CJNumberFunction.h>
#include <CJObjectFunction.h>
#include <CJObjectTypeFunction.h>
#include <CJRandFunction.h>
#include <CJRegExpFunction.h>
#include <CJErrorFunction.h>
#include <CJDateFunction.h>
#include <CJStringFunction.h>
#include <CJTypeFunction.h>
#include <CJObjTypeFunction.h>
#include <CJFunction.h>
#include <CJAlertFunction.h>
#include <CJIntervalFunction.h>
#include <CJRequireFunction.h>

#include <CJExecArray.h>
#include <CJExecAssert.h>
#include <CJExecAssignExpression.h>
#include <CJExecBreak.h>
#include <CJExecConst.h>
#include <CJExecContinue.h>
#include <CJExecDictionary.h>
#include <CJExecDo.h>
#include <CJExecExpression.h>
#include <CJExecExpressionList.h>
#include <CJExecFor.h>
#include <CJExecFunction.h>
#include <CJExecIncrDecrExpression.h>
#include <CJExecIdentifiers.h>
#include <CJExecIf.h>
#include <CJExecIndexExpression.h>
#include <CJExecLabel.h>
#include <CJExecNew.h>
#include <CJExecQuestion.h>
#include <CJExecReturn.h>
#include <CJExecSwitch.h>
#include <CJExecThis.h>
#include <CJExecThrow.h>
#include <CJExecTry.h>
#include <CJExecVar.h>
#include <CJExecWhile.h>
#include <CJExecWith.h>

#include <CFile.h>
#include <CStrUtil.h>
#include <CStrParse.h>
#include <CUtf8.h>
#include <iostream>
#include <cmath>

CJavaScript::
CJavaScript()
{
  initNameMaps();

  //------

  // Types (prototype ?)
  rootScope_    = CJDictionaryP(new CJDictionary(this, "__root__"));
  currentScope_ = rootScope_;

  //------

  // Global
  setProperty("alert"        , CJValueP(new CJAlertFunction  (this)));
  setProperty("setInterval"  , CJValueP(new CJSetInterval    (this)));
  setProperty("clearInterval", CJValueP(new CJClearInterval  (this)));
  setProperty("require"      , CJValueP(new CJRequireFunction(this)));

  setProperty("undefined", createUndefinedValue());
  setProperty("null"     , createNullValue());

  setProperty("NaN"     , CJValueP(new CJNumber(this, CJUtil::getNaN())));
  setProperty("Infinity", CJValueP(new CJNumber(this, CJUtil::getPosInf())));

  //------

  defineObjectT<CJStringFunction  , CJString  >(this, "String"  );
  defineObjectT<CJNumberFunction  , CJNumber  >(this, "Number"  );
  defineObjectT<CJBooleanFunction , CJBoolean >(this, "Boolean" );
  defineObjectT<CJArrayFunction   , CJArray   >(this, "Array"   );
  defineObjectT<CJObjectFunction  , CJObject  >(this, "Object"  );
  defineObjectT<CJFunctionFunction, CJFunction>(this, "Function");
  defineObjectT<CJDateFunction    , CJDate    >(this, "Date"    );
  defineObjectT<CJRegExpFunction  , CJRegExp  >(this, "RegExp"  );

  // Symbol

  // Math Dictionary (Object)
  math_ = CJMathP(new CJMath(this));
  // JSON Dictionary (Object)
  json_ = CJJSONP(new CJJSON(this));

  // System
  setProperty("System", CJValueP(new CJSystem(this)));

  // Int8Array, Uint8Array, Int8ClampedArray, Int16Array, Uint16Array,
  // Int32Array, Uint32Array, Float32Array, Float64Array

  // Map, Set, WeakMap, WeakSet

  // ArrayBuffer, DataView, JSON

  // Promise, Generator, GeneratorFunction

  // Intl, ...

  // Error, EvalError, InternalError, RangeError,
  // ReferenceError, SyntaxError, TypeError, URIError
  defineObjectT<CJErrorFunction         , CJError         >(this, "Error"         );
  defineObjectT<CJEvalErrorFunction     , CJEvalError     >(this, "EvalError"     );
  defineObjectT<CJRangeErrorFunction    , CJRangeError    >(this, "RangeError"    );
  defineObjectT<CJReferenceErrorFunction, CJReferenceError>(this, "ReferenceError");
  defineObjectT<CJSyntaxErrorFunction   , CJSyntaxError   >(this, "SyntaxError"   );
  defineObjectT<CJTypeErrorFunction     , CJTypeError     >(this, "TypeError"     );
  defineObjectT<CJURIErrorFunction      , CJURIError      >(this, "URIError"      );

  //------

  setProperty("eval"      , CJValueP(new CJGlobalFunction(this, "eval"      )));
  setProperty("isFinite"  , CJValueP(new CJGlobalFunction(this, "isFinite"  )));
  setProperty("isNaN"     , CJValueP(new CJGlobalFunction(this, "isNaN"     )));
  setProperty("parseFloat", CJValueP(new CJGlobalFunction(this, "parseFloat")));
  setProperty("parseInt"  , CJValueP(new CJGlobalFunction(this, "parseInt"  )));

  //------

  // document object
  setProperty("document", CJValueP(new CJDocumentObject(this)));

  // console object
  setProperty("console", CJValueP(new CJConsoleObject(this)));

  // debug object
  setProperty("debug", CJValueP(new CJDebugObject(this)));
}

void
CJavaScript::
initNameMaps()
{
  nameOperatorMap_["delete"    ] = CJOperator::Type::Delete;
  nameOperatorMap_["in"        ] = CJOperator::Type::In;
  nameOperatorMap_["instanceof"] = CJOperator::Type::InstanceOf;
  nameOperatorMap_["typeof"    ] = CJOperator::Type::TypeOf;
  nameOperatorMap_["void"      ] = CJOperator::Type::Void;

  nameRealTypeMap_["NaN"      ] = CJNumber::RealType::NaN;
  nameRealTypeMap_["Infinity" ] = CJNumber::RealType::PosInf;
  nameRealTypeMap_["-Infinity"] = CJNumber::RealType::NegInf;
}

void
CJavaScript::
defineObject(const std::string &name, CJObjTypeFunctionP typeFn, CJObjP prototype)
{
  setProperty(name, typeFn);

  typeFn->setProperty(this, "prototype", prototype);

  //CJDictionaryP prototype = std::static_pointer_cast<CJDictionary>(
  //  typeFn->getProperty(this, "prototype"));

  // <Object>.prototype.constructor = <ObjectConstructor>
  if (prototype)
    prototype->setProperty(this, "constructor", typeFn);

  typeFn->objectType()->setTypeFunction(typeFn);

  CJObjTypeP typeFnObjType = typeFn->objectType();

  // add object functions to prototype
  for (const auto &f : typeFnObjType->objFunctions()) {
    if (prototype)
      prototype->setProperty(this, f.first, f.second);
  }
}

void
CJavaScript::
loadStartpFile()
{
  const char *homeDir = getenv("HOME");

  if (! homeDir)
    return;

  std::string fileName = std::string(homeDir) + "/.CJavaScript";

  (void) loadSubFile(fileName);
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
  scopeStack_.push_back(currentScope());

  scope->setParent(currentScope());

  setCurrentScope(scope);
//printScopeStack("startScope");
}

CJDictionaryP
CJavaScript::
endScope()
{
  CJDictionaryP scope = currentScope();

  setCurrentScope(scopeStack_.back());

  scopeStack_.pop_back();

  scope->setParent(0);

//printScopeStack("endScope");
  return scope;
}

void
CJavaScript::
startFunctionScope(CJFunctionP fn)
{
  functions_.push_back(fn);

  CJDictionaryP scope = fn->scope();

  if (getenv("JS_PRINT_FUNCTION_SCOPE"))
    printScopeChain(scope, "startFunctionScope [proc " + scope->name() + "]");

  currentScopeStack_.push_back(currentScope());
  scopeStackStack_  .push_back(scopeStack_);

  initFunctionScope(fn);

  setCurrentScope(fn->scope());

  scopeStack_.clear();

  if (getenv("JS_PRINT_FUNCTION_SCOPE"))
    printScopeStack("startFunctionScope [stack]");
}

CJFunctionP
CJavaScript::
endFunctionScope()
{
  if (getenv("JS_PRINT_FUNCTION_SCOPE"))
    printScopeStack("endFunctionScope [stack]");

  CJDictionaryP scope = currentScope();

  setCurrentScope(currentScopeStack_.back());

  scopeStack_ = scopeStackStack_.back();

  currentScopeStack_.pop_back();
  scopeStackStack_  .pop_back();

  CJFunctionP fn = functions_.back();

  functions_.pop_back();

  termFunctionScope(fn);

  if (! functions_.empty()) {
    CJFunctionP fn = functions_.back();

    initFunctionScope(fn);
  }

  if (getenv("JS_PRINT_FUNCTION_SCOPE"))
    printScopeChain(currentScope(), "endFunctionScope [proc " + scope->name() + "]");

  return fn;
}

void
CJavaScript::
initFunctionScope(CJFunctionP fn)
{
  CJDictionaryP rootScope = fn->scope();

  if (! rootScope)
    return;

  while (rootScope->getParent())
    rootScope = rootScope->getParent();

  if (rootScope.get() != rootScope_.get())
    rootScope->setParent(rootScope_);
}

void
CJavaScript::
termFunctionScope(CJFunctionP fn)
{
  CJDictionaryP rootScope = fn->scope();

  if (! rootScope)
    return;

  while (rootScope->getParent())
    rootScope = rootScope->getParent();

  rootScope->setParent(CJDictionaryP());
}

CJFunctionP
CJavaScript::
currentFunction() const
{
  if (functions_.empty())
    return CJFunctionP();

  return functions_.back();
}

void
CJavaScript::
printScopeStack(const std::string &msg) const
{
  std::cout << msg << " " << currentScope()->name();

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
pushUserFunction(CJFunctionP fn)
{
  userFunctions_.push_back(fn);
}

void
CJavaScript::
popUserFunction()
{
  userFunctions_.pop_back();
}

CJDictionaryP
CJavaScript::
currentUserFunction() const
{
  CJDictionaryP scope;

  if (userFunctions_.empty())
    return CJDictionaryP();

  return userFunctions_.back()->scope();
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
    if (block->isBreakFlag()) {
      if (block_)
        block_->setBreakFlag(true);
    }

    if (block->isContinueFlag()) {
      if (block_)
        block_->setContinueFlag(true);
    }

    if (block->isReturnFlag()) {
      if (block_) {
        block_->setReturnFlag(true);
        block_->setRetVal(block->getRetVal());
      }
    }
  }
  else if (block->type() == CJExecBlock::Type::Switch) {
    // no propagate break for switch

    if (block->isContinueFlag()) {
      if (block_)
        block_->setContinueFlag(true);
    }

    if (block->isReturnFlag()) {
      if (block_) {
        block_->setReturnFlag(true);
        block_->setRetVal(block->getRetVal());
      }
    }
  }
  else if (block->type() == CJExecBlock::Type::Iterative) {
    if (block->getRetVal()) {
      if (block_)
        block_->setRetVal(block->getRetVal());
    }
  }

  return block;
}

CJDictionaryP
CJavaScript::
thisScope() const
{
  if (! thisStack_.empty())
    return thisStack_.back();
  else
    return rootScope_;
}

void
CJavaScript::
pushThis(CJDictionaryP scope)
{
  thisStack_.push_back(scope);
}

void
CJavaScript::
popThis()
{
  thisStack_.pop_back();
}

CJValueP
CJavaScript::
setProperty(const std::string &name, CJValueP value)
{
  currentScope()->setProperty(this, name, value);

  return value;
}

bool
CJavaScript::
indexValue(CJValueP value, CJValueP ivalue, CJValueP &rvalue) const
{
  if      (value->hasIndex()) {
    long ind = ivalue->toInteger();

    rvalue = value->indexValue(ind);
  }
  else if (value->hasProperty()) {
    std::string ind = ivalue->toString();

    rvalue = value->propertyValue(ind);
  }
  else {
    rvalue = CJValueP();
    return false;
  }

  return true;
}

bool
CJavaScript::
setIndexValue(CJValueP value, CJValueP ivalue, CJValueP rvalue)
{
  if      (value->hasIndex()) {
    long ind = ivalue->toInteger();

    value->setIndexValue(ind, rvalue);
  }
  else if (value->hasProperty()) {
    std::string ind = ivalue->toString();

    value->setPropertyValue(ind, rvalue);
  }
  else
    return false;

  return true;
}

bool
CJavaScript::
deleteIndexValue(CJValueP value, CJValueP ivalue)
{
  if (value->type() == CJToken::Type::String)
    return false;

  if      (value->hasIndex()) {
    long ind = ivalue->toInteger();

    value->deleteIndexValue(ind);
  }
  else if (value->hasProperty()) {
    std::string ind = ivalue->toString();

    value->deletePropertyValue(ind);
  }
  else
    return false;

  return true;
}

//---

bool
CJavaScript::
lookupPropertyData(const Identifiers &identifiers, CJPropertyData &data)
{
  bool create = data.isCreate();

  data.setCreate(false);

  CJDictionaryP scope = currentScope();

  if (lookupScopePropertyData(scope, identifiers, data, 0))
    return true;

  while (scope->getParent()) {
    scope = scope->getParent();

    if (lookupScopePropertyData(scope, identifiers, data, 0))
      return true;
  }

  if (create) {
    data.setCreate(true);

    CJDictionaryP scope = currentScope();

    if (lookupScopePropertyData(scope, identifiers, data, 0))
      return true;

    while (scope->getParent()) {
      scope = scope->getParent();

      if (lookupScopePropertyData(scope, identifiers, data, 0))
        return true;
    }
  }

  return false;
}

bool
CJavaScript::
lookupScopePropertyData(CJDictionaryP dict, const Identifiers &identifiers,
                        CJPropertyData &data, int ind)
{
  CJNameSpaceP scope = std::static_pointer_cast<CJNameSpace>(dict);
  CJValueP     value = std::static_pointer_cast<CJValue    >(dict);

  return lookupScopeValuePropertyData(scope, value, identifiers, data, ind);
}

bool
CJavaScript::
lookupFunctionPropertyData(CJFunctionBaseP fn, const Identifiers &identifiers,
                           CJPropertyData &data, int ind)
{
  CJNameSpaceP scope = std::static_pointer_cast<CJNameSpace>(fn);
  CJValueP     value = std::static_pointer_cast<CJValue    >(fn);

  return lookupScopeValuePropertyData(scope, value, identifiers, data, ind);
}

bool
CJavaScript::
lookupScopeValuePropertyData(CJNameSpaceP scope, CJValueP svalue, const Identifiers &identifiers,
                             CJPropertyData &data, int ind)
{
  int len = identifiers.size();

  if (ind >= len)
    return false;

  // get property
  const std::string &name = identifiers[ind]->name();

  // non-last name so recurse
  if (ind < len - 1) {
    // get object value
    CJValueP value = scope->getProperty(this, name);

    if (! value)
      return false;

    // lookup remaining names in value
    return lookupValuePropertyData(value, identifiers, data, ind + 1);
  }

  //---

  // get last name value
  // TODO: use hasProperty
  CJValueP value = scope->getProperty(this, name);

  // fail if no value and no create
  if (! value && ! data.isCreate())
    return false;

  // set parent scope, scope value and property name
  data.setScope   (scope);
  data.setObjValue(svalue);
  data.setName    (name);

  if (! data.isModifiable()) {
    // create dummy value for create
    if (! value && data.isCreate()) {
      // auto create with null value if create enabled
      // TODO: undefined ?
      //std::cerr << "Set property '" << name << "' for '" << scope->name() << "'" << std::endl;
      value = createNullValue();

      scope->setProperty(this, name, value);
    }

    // set value
    data.setValue(value);
  }

  return true;
}

bool
CJavaScript::
lookupObjPropertyData(CJObjP obj, const Identifiers &identifiers,
                      CJPropertyData &data, int ind)
{
  int len = identifiers.size();

  if (ind >= len)
    return false;

  const std::string &name = identifiers[ind]->name();

  // non-last name so recurse
  if (ind < len - 1) {
    // get object value
    CJValueP value = obj->getProperty(this, name);

    if (! value)
      return false;

    // lookup remaining names in value
    return lookupValuePropertyData(value, identifiers, data, ind + 1);
  }

  //---

  // get last name value
  // TODO: use hasProperty
  CJValueP value = obj->getProperty(this, name);

  // fail if no value and no create
  if (! value && ! data.isCreate())
    return false;

  // get last name value with details of parent
  data.setObj     (obj);
  data.setObjValue(obj);
  data.setName    (name);

  // lvalue
  if (! data.isModifiable()) {
    // create dummy value for create
    if (! value && data.isCreate()) {
      // auto create with null value if create enabled
      // TODO: undefined ?
      //std::cerr << "Set property '" << name << "' for '" << scope->name() << "'" << std::endl;
      value = createNullValue();

      obj->setProperty(this, name, value);
    }

    data.setValue(value);
  }

  return true;
}

bool
CJavaScript::
lookupValuePropertyData(CJValueP value, const Identifiers &identifiers,
                        CJPropertyData &data, int ind)
{
  int len = identifiers.size();

  if (ind >= len)
    return false;

  // TODO: lookup order ???

  // lookup in object
  if      (value->isObject()) {
    CJObjP obj1 = std::static_pointer_cast<CJObj>(value);

    return lookupObjPropertyData(obj1, identifiers, data, ind);
  }
  // lookup in function
  else if (value->isFunction()) {
    CJFunctionBaseP fn1 = std::static_pointer_cast<CJFunctionBase>(value);

    return lookupScopePropertyData(fn1, identifiers, data, ind);
  }
  // lookup in dictionary
  else if (value->type() == CJToken::Type::Dictionary) {
    CJDictionaryP dict1 = std::static_pointer_cast<CJDictionary>(value);

    return lookupScopePropertyData(dict1, identifiers, data, ind);
  }
#if 0
  else if (value->isFunction()) {
    CJFunctionBaseP fn1 = std::static_pointer_cast<CJFunctionBase>(value);

    if      (fn1->type() == CJFunctionBase::Type::User) {
      CJFunctionP userFn1 = std::static_pointer_cast<CJFunction>(value);

      return lookupScopePropertyData(userFn1->scope(), identifiers, data, ind);
    }
    else if (fn1->type() == CJFunctionBase::Type::ObjType ||
             fn1->type() == CJFunctionBase::Type::ObjectType ||
             fn1->type() == CJFunctionBase::Type::Object) {
      if (ind != len - 1) {
        throwSyntaxError(0, "Invalid object type identifiers");
        return false;
      }

      CJObjTypeP valueType1;

      if (fn1->type() == CJFunctionBase::Type::ObjType) {
        CJTypeFunctionP typeFn = std::static_pointer_cast<CJTypeFunction>(value);

        valueType1 = typeFn->objectType();
      }
      if (fn1->type() == CJFunctionBase::Type::ObjectType) {
        CJObjectTypeFunctionP objTypeFn = std::static_pointer_cast<CJObjectTypeFunction>(value);

        valueType1 = objTypeFn->objectType();
      }
      else {
        //CJObjectFunctionP objFn = std::static_pointer_cast<CJObjectFunction>(value);
      }

      const std::string &name1 = identifiers[ind]->name();

      CJValueP propVal;

      if (valueType1 && valueType1->hasProperty(this, name1))
        propVal = valueType1->getProperty(this, name1);

      if (! propVal)
        return false;

      if (propVal->isFunction()) {
        CJTypeObjectFunctionP fn(new CJTypeObjectFunction(this, valueType1, name1));

        data.setObjValue(value);
        data.setValue   (std::static_pointer_cast<CJValue>(fn));

        return true;
      }
      else {
        // TODO: namespace function
        data.setValue(propVal);

#if 0
        CJObjType::Values values;

        values.push_back(value);

        data.setValue(valueType1->exec(this, name1, values));
#endif

        return true;
      }
    }
    else
      return false;
  }
#endif
  // true, false are values not objects so promote to Boolean value before processing
  else if (value->type() == CJToken::Type::True || value->type() == CJToken::Type::False) {
    CJValueP value1(new CJBoolean(this, value->toBoolean()));

    return lookupValuePropertyData(value1, identifiers, data, ind);
  }
  // lookup object property
  // TODO: support multiple names
  // TODO: used ??
  else {
    // value is an object
    if (ind != len - 1) {
      throwSyntaxError(0, "Invalid object identifiers");
      return false;
    }

    const std::string &name1 = identifiers[ind]->name();

    CJValueP propVal;

    CJObjTypeP valueType = value->valueType();

    if (valueType->hasProperty(this, name1))
      propVal = valueType->getProperty(this, name1);

    if (! propVal)
      return false;

    if (propVal->isFunction()) {
      CJTypeObjectFunctionP fn(new CJTypeObjectFunction(this, valueType, name1));

      data.setObjValue(value);
      data.setValue   (std::static_pointer_cast<CJValue>(fn));

      return true;
    }
    else {
      CJObjType::Values values;

      values.push_back(value);

      data.setValue(valueType->exec(this, name1, values));

      return true;
    }
  }
}

//---

CJValueP
CJavaScript::
valueToObj(CJValueP value) const
{
  CJValueP value1;

  if (value->type() == CJToken::Type::True || value->type() == CJToken::Type::False) {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    value1 = CJValueP(new CJBoolean(th, value->toBoolean()));
  }

  return value1;
}

//---

CJValueP
CJavaScript::
lookupValue(const Identifiers &identifiers)
{
  CJPropertyData data(this);

  if (! lookupPropertyData(identifiers, data))
    return CJValueP();

  return data.value();
}

//---

CJLValueP
CJavaScript::
lookupLValue(const Identifiers &identifiers)
{
  CJPropertyData data(this);

  data.setModifiable(true);

  if (! lookupPropertyData(identifiers, data))
    return CJLValueP();

  return data.lvalue();
}

//---

CJavaScript::ValuePair
CJavaScript::
lookupObjectProperty(const Identifiers &identifiers, bool create)
{
  CJPropertyData data(this);

  data.setModifiable(true);
  data.setCreate    (create);

  if (! lookupPropertyData(identifiers, data))
    return ValuePair();

  CJValueP objValue, dataValue;

  if      (data.obj())
    objValue = std::static_pointer_cast<CJValue>(data.obj());
  else if (data.objValue())
    objValue = data.objValue();

  dataValue = data.value();

  return ValuePair(objValue, dataValue);
}

//---

CJLValueP
CJavaScript::
lookupProperty(const Identifiers &identifiers, bool create)
{
  CJPropertyData data(this);

  data.setModifiable(true);
  data.setCreate    (create);

  if (! lookupPropertyData(identifiers, data))
    return CJLValueP();

  return data.lvalue();
}

//---

bool
CJavaScript::
deleteToken(const Tokens &tokens)
{
  assert(tokens.size() == 1);

  CJTokenP token = tokens[0];

  if      (token->type() == CJToken::Type::Identifiers) {
    CJExecIdentifiersP identifiers = std::static_pointer_cast<CJExecIdentifiers>(token);

    return deleteProperty(identifiers->identifiers());
  }
  else if (token->type() == CJToken::Type::IndexExpression) {
    std::deque<CJValueP> values;

    CJExecIndexExpressionP iexpr = std::static_pointer_cast<CJExecIndexExpression>(token);

    CJExecIdentifiersP identifiers = iexpr->identifiers();

    while (! identifiers) {
      values.push_front(iexpr->indValue(this));

      CJExecIndexExpressionP liexpr = iexpr->lindexExpression();

      if (! liexpr)
        return true;

      identifiers = liexpr->identifiers();

      iexpr = liexpr;
    }

    values.push_front(iexpr->indValue(this));

    Values values1;

    for (const auto &v : values)
      values1.push_back(v);

    return deleteProperty(identifiers->identifiers(), values1);
  }
  else
    return true;

  return true;
}

bool
CJavaScript::
deleteProperty(const Identifiers &identifiers, const Values &values)
{
  CJDictionaryP scope = currentScope();

  if (deleteProperty(scope, identifiers, values))
    return true;

  while (scope->getParent()) {
    scope = scope->getParent();

    if (deleteProperty(scope, identifiers, values))
      return true;
  }

  return false;
}

bool
CJavaScript::
deleteProperty(CJDictionaryP scope, const Identifiers &identifiers, const Values &values)
{
  int len = identifiers.size();

  for (int i = 0; i < len; ++i) {
    if (i < len - 1) {
      CJValueP varValue = scope->getProperty(this, identifiers[i]->name());

      if (varValue) {
        ++i;

        if (varValue->type() == CJToken::Type::Dictionary) {
          CJDictionaryP dict = std::static_pointer_cast<CJDictionary>(varValue);

          CJValueP value = dict->getProperty(this, identifiers[i]->name());

          while (value && value->type() == CJToken::Type::Dictionary && i < len - 1) {
            ++i;

            dict = std::static_pointer_cast<CJDictionary>(value);

            value = dict->getProperty(this, identifiers[i]->name());
          }

          if (i < len - 1)
            throwSyntaxError(0, "Invalid value named index");

          if (dict) {
            dict->deleteProperty(this, identifiers[i]->name(), values);
            return true;
          }
        }
      }
    }
    else {
      if (values.empty() && ! scope->canDeleteProperty(identifiers[i]->name()))
        return false;

      scope->deleteProperty(this, identifiers[i]->name(), values);

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

CJObjTypeP
CJavaScript::
getTypeObject(CJToken::Type type) const
{
  auto p = typeObject_.find(type);

  if (p == typeObject_.end())
    return CJObjTypeP();

  return (*p).second;
}

int
CJavaScript::
isCompleteLine(const std::string &str) const
{
  if (! str.empty()) {
    if (str[str.size() - 1] == '\\')
      return 1;
  }

  //---

  // check for complete line (matching {}, []) - returns depth

  int numBraces   = 0;
  int numSBracket = 0;
  int numRBracket = 0;
  int numComment  = 0;

  CStrParse parse(str);

  while (! parse.eof()) {
    parse.skipSpace();

    if      (parse.isChar('\"') || parse.isChar('\''))
      parse.skipString();
    else if (parse.isChar('{')) {
      ++numBraces; parse.skipChar();
    }
    else if (parse.isChar('}')) {
      --numBraces; parse.skipChar();
    }
    else if (parse.isChar('[')) {
      ++numSBracket; parse.skipChar();
    }
    else if (parse.isChar(']')) {
      --numSBracket; parse.skipChar();
    }
    else if (parse.isChar('(')) {
      ++numRBracket; parse.skipChar();
    }
    else if (parse.isChar(')')) {
      --numRBracket; parse.skipChar();
    }
    else if (parse.isString("/*")) {
      parse.skipChars("/*");

      while (! parse.eof() && ! parse.isString("*/"))
        parse.skipChar();

      if (! parse.isString("*/"))
        ++numComment;
    }
    else
      parse.skipChar();
  }

  return (numBraces + numSBracket + numRBracket + numComment);
}

bool
CJavaScript::
loadFile(const std::string &filename)
{
  if (! CFile::isRegular(filename))
    return false;

  //---

  fileName_ = filename;

  CFile file(filename);

  std::string str;

  std::string line;

  while (file.readLine(line)) {
    line = CStrUtil::stripSpaces(line);

    if (! str.empty())
      str += "\n";

    str += line;
  }

  return loadText(str);
}

bool
CJavaScript::
loadString(const std::string &str)
{
  fileName_ = "";

  return loadText(str);
}

bool
CJavaScript::
loadText(const std::string &str)
{
  tokens_.clear();

  parseString(str);

  if (isParseDebug()) {
    for (const auto &t : tokens_)
      std::cout << *t << "@" << t->lineNum() << std::endl;
  }

  return true;
}

void
CJavaScript::
parseString(const std::string &str)
{
  CJOperator::Type   opType;
  CJNumber::RealType realType;

  CStrParse parse(str);

  parse.skipSpace();

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

    std::string name = getIdentifier(parse, allowUnary);

    if (isOperatorName(name, opType)) {
      CJOperatorP op;

      if      (opType == CJOperator::Type::Delete) {
        parse.skipChars("delete");

        op = CJOperatorP(new CJOperator(CJOperator::Type::Delete, 1,
          CJOperator::Associativty::Right, CJOperator::Ary::Unary));
      }
      else if (opType == CJOperator::Type::In) {
        parse.skipChars("in");

        op = CJOperatorP(new CJOperator(CJOperator::Type::In, 11,
          CJOperator::Associativty::Left, CJOperator::Ary::Binary));
      }
      else if (opType == CJOperator::Type::InstanceOf) {
        parse.skipChars("instanceof");

        op = CJOperatorP(new CJOperator(CJOperator::Type::InstanceOf, 11,
          CJOperator::Associativty::Left, CJOperator::Ary::Binary));
      }
      else if (opType == CJOperator::Type::TypeOf) {
        parse.skipChars("typeof");

        op = CJOperatorP(new CJOperator(CJOperator::Type::TypeOf, 1,
          CJOperator::Associativty::Right, CJOperator::Ary::Unary));
      }
      else if (opType == CJOperator::Type::Void) {
        parse.skipChars("void");

        op = CJOperatorP(new CJOperator(CJOperator::Type::Void, 1,
          CJOperator::Associativty::Right, CJOperator::Ary::Unary));
      }
      else
        assert(false);

      op->setLineNum(parse.lineNum());

      tokens_.push_back(CJTokenP(op));
    }
    else if (isRealValueName(name, allowUnary, realType)) {
      CJTokenP token;

      if      (realType == CJNumber::RealType::NaN) {
        parse.skipChars("NaN");

        token = createNumberValue(CJUtil::getNaN());
      }
      else if (realType == CJNumber::RealType::PosInf) {
        parse.skipChars("Infinity");

        token = createNumberValue(CJUtil::getPosInf());
      }
      else if (realType == CJNumber::RealType::NegInf) {
        parse.skipChars("-Infinity");

        token = createNumberValue(CJUtil::getNegInf());
      }
      else
        assert(false);

      token->setLineNum(parse.lineNum());

      tokens_.push_back(token);
    }
    else if (parse.isAlpha() || parse.isOneOf("_$")) {
      readIdentifier(parse);
    }
    else if (parse.isDigit()) {
      readNumber(parse);
    }
    else if (parse.isOneOf("+-.") && parse.isDigitAt(1) && allowUnary) {
      readNumber(parse);
    }
    else if (parse.isString("//")) {
      // skip to end of line
      parse.skipChars("//");

      while (! parse.eof() && ! parse.isChar('\n'))
        parse.skipChar();
    }
    else if (parse.isString("/*")) {
      // skip to */
      parse.skipChars("/*");

      while (! parse.eof() && ! parse.isString("*/"))
        parse.skipChar();

      if (parse.isString("*/"))
        parse.skipChars("*/");
    }
    else if (allowUnary && parse.isChar('/')) {
      readRegExp(parse);
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
      throwSyntaxError(0, "Invalid text: '" + parse.getAt() + "'");
      break;
    }

    parse.skipSpace();
  }
}

CJValueP
CJavaScript::
exec()
{
  CJExecData execData;

  execData.initExec(tokens_);

  interp(execData);

  ETokens etokens = execData.etokens();

  CJValueP value;

  for (auto &t : etokens) {
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

CJExecBlockP
CJavaScript::
interpFunctionBlock(const std::string &str)
{
  if (! loadText(str))
    return CJExecBlockP();

  CJExecData execData;

  execData.initExec(tokens_);

  execDataStack_.push_back(execData_);

  execData_ = &execData;

  //---

  CJExecBlockP block(new CJExecBlock(CJExecBlock::Type::Function));

  block->setLineNum(execLineNum());

  while (! execData_->eof()) {
    CJTokenP token = execData_->token();

    block->addToken(token);

    execData_->next();
  }

  //---

  block->interp(this);

  if (isInterpDebug()) {
    std::cerr << "interpFunctionBlock: " << *block << std::endl;
  }

  //---

  execData_ = execDataStack_.back();

  execDataStack_.pop_back();

  return block;
}

CJValueP
CJavaScript::
interpString(const std::string &str)
{
  if (! loadText(str))
    return CJValueP();

  return exec();
}

bool
CJavaScript::
loadSubFile(const std::string &filename)
{
  std::string fileName = fileName_;

  if (! loadFile(filename)) {
    fileName_ = fileName;
    return false;
  }

  CJExecData execData;

  execData.initExec(tokens_);

  interp(execData);

  fileName_ = fileName;

  return true;
}

void
CJavaScript::
interp(CJExecData &execData)
{
  execDataStack_.push_back(execData_);

  execData_ = &execData;

  while (! execData_->eof()) {
    CJTokenP token = execData_->token();

    CJToken::Type type = token->type();

    if      (type == CJToken::Type::Identifier) {
      if (isExecLabel()) {
        CJExecLabelP label = interpExecLabel();

        if (! label) {
          std::ostringstream ss; ss << *token;
          throwSyntaxError(token, "Interp failed at token: " + ss.str());
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(label));
      }
      else {
        CJExecExpressionListP exprList = interpExpressionList();

        if (! exprList) {
          std::ostringstream ss; ss << *token;
          throwSyntaxError(token, "Interp failed at token: " + ss.str());
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(exprList));
      }
    }
    else if (type == CJToken::Type::Operator) {
      CJOperator *op = token->cast<CJOperator>();

      CJOperator::Type opType = op->type();

      if (opType == CJOperator::Type::Colon) {
        if (! execData_->isBlock()) {
          std::ostringstream ss; ss << *token;
          throwSyntaxError(token, "Interp failed at operator: " + ss.str());
          break;
        }

        execData_->next();

        //---

        CJValueP key = execData_->unstackEValue(this);

        if (! key) {
          throwSyntaxError(token, "Left hand side value missing for colon");
          break;
        }

        CJExecExpressionP valueExpr = interpExpression();

        if (! valueExpr) {
          throwSyntaxError(token, "Missing expression after colon");
          break;
        }

        //---

        CJExecDictionaryP dict(new CJExecDictionary);

        dict->setLineNum(key->lineNum());

        dict->addDictionaryValue(key, valueExpr);

        //---

        while (isExecOperator(CJOperator::Type::Comma)) {
          execData_->next();

          CJExecExpressionP keyExpr = interpExpression();

          if (! keyExpr) {
            throwSyntaxError(token, "Missing key expression for dictionary");
            break;
          }

          CJValueP key = keyExpr->exec(this);

          //---

          if (! interpExecOperator(CJOperator::Type::Colon)) {
            throwSyntaxError(token, "Missing colon after key for dictionary");
            break;
          }

          CJExecExpressionP valueExpr = interpExpression();

          if (! valueExpr) {
            throwSyntaxError(token, "Missing key expression for dictionary");
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
        throwSyntaxError(token, "Interp failed at operator: " + ss.str());
        break;
      }

      CJExecExpressionListP exprList = interpExpressionList();

      if (! exprList) {
        std::ostringstream ss; ss << *token;
        throwSyntaxError(token, "Interp failed at token: " + ss.str());
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
        throwSyntaxError(token, "Interp failed at token: " + ss.str());
        break;
      }

      execData_->addEToken(std::static_pointer_cast<CJToken>(exprList));
    }
    else if (type == CJToken::Type::Keyword) {
      CJKeyword *keyword = token->cast<CJKeyword>();

      if      (keyword->type() == CJKeyword::Type::This) {
        CJExecThisP ethis = interpExecThis();

        execData_->addEToken(std::static_pointer_cast<CJToken>(ethis));
      }
      // for ( <LexicalDeclaration> <Expression>(opt) ; <Expression>(opt) <Statement>
      else if (keyword->type() == CJKeyword::Type::For) {
        CJExecForP efor = interpExecFor();

        if (! efor) {
          throwSyntaxError(token, "Interp failed for 'for'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(efor));
      }
      // if (<expression>) <statement>
      //   [ else [ if (<expression>) <statement> ... ]
      //   [ else <statement>] ]
      else if (keyword->type() == CJKeyword::Type::If) {
        CJExecIfP eif = interpExecIf();

        if (! eif) {
          throwSyntaxError(token, "Interp failed for 'if'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(eif));
      }
      // while ( <expression> ) <statement>
      else if (keyword->type() == CJKeyword::Type::While) {
        CJExecWhileP ewhile = interpExecWhile();

        if (! ewhile) {
          throwSyntaxError(token, "Interp failed for 'while'");
          break;
        }

        execData_->addEToken(CJTokenP(ewhile));
      }
      // do <statement> while ( <expression> )
      else if (keyword->type() == CJKeyword::Type::Do) {
        CJExecDoP edo = interpExecDo();

        if (! edo) {
          throwSyntaxError(token, "Interp failed for 'do'");
          break;
        }

        execData_->addEToken(CJTokenP(edo));
      }
      // switch ( <expression> ) {
      //   [ case <expression> : <statementList> ]
      //   [ default : <statementList> ]
      // }
      else if (keyword->type() == CJKeyword::Type::Switch) {
        CJExecSwitchP eswitch = interpExecSwitch();

        if (! eswitch) {
          throwSyntaxError(token, "Interp failed for 'switch'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(eswitch));
      }
      else if (keyword->type() == CJKeyword::Type::Var) {
        execData_->next();

        CJExecVarP evar = interpExecVar();

        if (! evar) {
          throwSyntaxError(token, "Interp failed for 'var'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(evar));
      }
      else if (keyword->type() == CJKeyword::Type::Const) {
        CJExecConstP econst = interpExecConst();

        if (! econst) {
          throwSyntaxError(token, "Interp failed for 'const'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(econst));
      }
      // with ( <expression> ) <statement>
      else if (keyword->type() == CJKeyword::Type::With) {
        CJExecWithP ewith = interpExecWith();

        if (! ewith) {
          throwSyntaxError(token, "Interp failed for 'with'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(ewith));
      }
      else if (keyword->type() == CJKeyword::Type::New) {
        CJExecNewP enew = interpExecNew();

        if (! enew) {
          throwSyntaxError(token, "Interp failed for 'new'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(enew));
      }
      // assert <expression_list>
      else if (keyword->type() == CJKeyword::Type::Assert) {
        CJExecAssertP eassert = interpExecAssert();

        if (! eassert) {
          throwSyntaxError(token, "Interp failed for 'assert'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(eassert));
      }
      // break [<label>]
      else if (keyword->type() == CJKeyword::Type::Break) {
        CJExecBreakP ebreak = interpExecBreak();

        if (! ebreak) {
          throwSyntaxError(token, "Interp failed for 'break'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(ebreak));
      }
      // continue [<label>]
      else if (keyword->type() == CJKeyword::Type::Continue) {
        CJExecContinueP econt = interpExecContinue();

        if (! econt) {
          throwSyntaxError(token, "Interp failed for 'continue'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(econt));
      }
      // return [<expression>]
      else if (keyword->type() == CJKeyword::Type::Return) {
        CJExecReturnP eret = interpExecReturn();

        if (! eret) {
          throwSyntaxError(token, "Interp failed for 'return'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(eret));
      }
      // function [<identifier>] ( <parameters> ) { <body> }
      else if (keyword->type() == CJKeyword::Type::Function) {
        CJFunctionP efunction = interpUserFunction(/*named*/true);

        if (! efunction) {
          throwSyntaxError(token, "Interp failed for 'function'");
          break;
        }

        CJDictionaryP scope = currentUserFunction();

        if (! scope) {
          CJDictionaryP scope = currentScope();

          scope->setProperty(this, efunction->name(), std::static_pointer_cast<CJValue>(efunction));
        }
        else {
          execData_->addEToken(std::static_pointer_cast<CJToken>(efunction));
        }
      }
      // try <block> [catch (<parameter>) <block>] [finally <block>]
      else if (keyword->type() == CJKeyword::Type::Try) {
        CJExecTryP etry = interpExecTry();

        if (! etry) {
          throwSyntaxError(token, "Interp failed for 'try'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(etry));
      }
      // throw <expression>
      else if (keyword->type() == CJKeyword::Type::Throw) {
        CJExecThrowP ethrow = interpExecThrow();

        if (! ethrow) {
          throwSyntaxError(token, "Interp failed for 'throw'");
          break;
        }

        execData_->addEToken(std::static_pointer_cast<CJToken>(ethrow));
      }
      else {
        throwSyntaxError(keyword, "Interp failed at keyword: " + keyword->name());
        break;
      }
    }
    else {
      std::stringstream ss; ss << *token;
      throwSyntaxError(token, "Interp failed at token: " + ss.str());
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

  //---

  if (! interpExecOperator(CJOperator::Type::OpenRBracket)) {
    throwSyntaxError(func, "Missing open round bracket");
    return CJExecFunctionP();
  }

  if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
    CJExecExpressionListP exprList = interpExpressionList();

    if (! exprList) {
      return CJExecFunctionP();
    }

    func->setExprList(exprList);

    if (! interpExecOperator(CJOperator::Type::CloseRBracket)) {
      throwSyntaxError(exprList, "Missing close round bracket");
      return CJExecFunctionP();
    }
  }
  else {
    execData_->next();
  }

  return func;
}

// for ( <LexicalDeclaration> <Expression>(opt) ; <Expression>(opt) <Statement>
// for ( <LexicalDeclaration> <Expression>(opt) in <Expression> <Statement>
CJExecForP
CJavaScript::
interpExecFor()
{
  CJExecForP efor(new CJExecFor);

  efor->setLineNum(execLineNum());

  //---

  // for
  if (! interpExecKeyword(CJKeyword::Type::For))
    return CJExecForP();

  // <<LexicalDeclaration>
  if (! interpExecOperator(CJOperator::Type::OpenRBracket)) {
    throwSyntaxError(efor, "Missing open round bracket");
    return CJExecForP();
  }

  bool isIn = false;

  // var <Expression>
  if (isExecKeyword(CJKeyword::Type::Var)) {
    execData_->next();

    // var <identifiers> in
    if (isInterpForIn()) {
      CJExecIdentifiersP identifiers = interpIdentifiers();

      if (! identifiers) {
        throwSyntaxError(efor, "Missing variable names for for in");
        return CJExecForP();
      }

      efor->setIdentifiers(identifiers, /*var*/true);

      isIn = true;
    }
    // var <Expression>
    else {
      CJExecVarP evar = interpExecVar();

      if (! evar) {
        throwSyntaxError(efor, "Interp failed for 'var'");
        return CJExecForP();
      }

      efor->setVar(evar);
    }
  }
  else {
    // <identifiers> in
    if (isInterpForIn()) {
      CJExecIdentifiersP identifiers = interpIdentifiers();

      if (! identifiers) {
        throwSyntaxError(efor, "Missing variable names for for in");
        return CJExecForP();
      }

      efor->setIdentifiers(identifiers, /*var*/true);

      isIn = true;
    }
    // <Expression>
    else {
      CJExecExpressionListP exprList1 = interpExpressionList();

      if (! exprList1) {
        return CJExecForP();
      }

      efor->setExprList1(exprList1);
    }
  }

  // in <expression>
  if (isIn) {
    if (! interpExecOperator(CJOperator::Type::In)) {
      throwSyntaxError(efor, "Missing in for for in");
      return CJExecForP();
    }

    CJExecExpressionP expr = interpExpression();

    if (! expr) {
      return CJExecForP();
    }

    efor->setInExpr(expr);
  }
  // <expression> ; <expression>
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

  if (! interpExecOperator(CJOperator::Type::CloseRBracket)) {
    throwSyntaxError(efor, "Missing close round bracket for for");
    return CJExecForP();
  }

  //---

  // { <statement> }
  if (! isExecOperator(CJOperator::Type::OpenBrace)) {
    throwSyntaxError(efor, "Missing open brace for for");
    return CJExecForP();
  }

  CJExecBlockP block = interpExecBlock(CJExecBlock::Type::Iterative);

  if (! block) {
    return CJExecForP();
  }

  efor->setBlock(block);

  //---

  if (isInterpDebug())
    std::cerr << "interpExecFor: " << *efor << std::endl;

  return efor;
}

bool
CJavaScript::
isInterpForIn() const
{
  int pos = execData_->pos();

  CJTokenP token = execData_->token();

  if (token->type() != CJToken::Type::Identifier)
    return false;

  execData_->next();

  if (! isExecOperator(CJOperator::Type::In)) {
    execData_->setPos(pos);
    return false;
  }

  execData_->setPos(pos);

  return true;
}

CJExecQuestionP
CJavaScript::
interpExecQuestion(CJExecExpressionP bexpr)
{
  CJExecQuestionP equestion(new CJExecQuestion(bexpr));

  equestion->setLineNum(execLineNum());

  //---

  CJExecExpressionP expr1 = interpExpression();

  if (! expr1) {
    return CJExecQuestionP();
  }

  equestion->setExpr1(expr1);

  if (! interpExecOperator(CJOperator::Type::Colon)) {
    throwSyntaxError(equestion, "Missing colon for ?:");
    return CJExecQuestionP();
  }

  CJExecExpressionP expr2 = interpExpression();

  if (! expr2) {
    return CJExecQuestionP();
  }

  equestion->setExpr2(expr2);

  if (isInterpDebug()) {
    std::cerr << "interpExecQuestion: " << *equestion << std::endl;
  }

  return equestion;
}

CJExecIfP
CJavaScript::
interpExecIf()
{
  // if ( <expression> ) <statement>
  // if ( <expression> ) <statement> else <statement>

  CJExecIfP eif(new CJExecIf);

  eif->setLineNum(execLineNum());

  //---

  // if
  if (! interpExecKeyword(CJKeyword::Type::If))
    return CJExecIfP();

  // ( <expression> )
  if (! interpExecOperator(CJOperator::Type::OpenRBracket)) {
    throwSyntaxError(eif, "Missing open bracket for if");
    return CJExecIfP();
  }

  CJExecExpressionListP exprList = interpExpressionList();

  if (! exprList) {
    return CJExecIfP();
  }

  if (! interpExecOperator(CJOperator::Type::CloseRBracket)) {
    throwSyntaxError(eif, "Missing close bracket for if");
    return CJExecIfP();
  }

  //---

  // <statement>
  CJExecBlockP ifBlock = interpExecBlock(CJExecBlock::Type::Sequential);

  if (! ifBlock) {
    return CJExecIfP();
  }

  eif->setIfBlock(exprList, ifBlock);

  //---

  // else <statement>
  // else if ( <expression> ) <statement>
  while (isExecKeyword(CJKeyword::Type::Else)) {
    execData_->next();

    // else if ( <expression> ) <statement>
    if (isExecKeyword(CJKeyword::Type::If)) {
      execData_->next();

      // ( <expression> )
      if (! interpExecOperator(CJOperator::Type::OpenRBracket)) {
        throwSyntaxError(eif, "Missing open bracket for else if");
        return CJExecIfP();
      }

      CJExecExpressionListP exprList = interpExpressionList();

      if (! exprList) {
        return CJExecIfP();
      }

      if (! interpExecOperator(CJOperator::Type::CloseRBracket)) {
        throwSyntaxError(eif, "Missing close bracket for else if");
        return CJExecIfP();
      }

      //---

      // <statement>
      CJExecBlockP elseIfBlock = interpExecBlock(CJExecBlock::Type::Sequential);

      if (! elseIfBlock) {
        return CJExecIfP();
      }

      eif->addElseIfBlock(exprList, elseIfBlock);
    }
    else {
      // <statement>
      CJExecBlockP elseBlock = interpExecBlock(CJExecBlock::Type::Sequential);

      if (! elseBlock) {
        return CJExecIfP();
      }

      eif->setElseBlock(elseBlock);

      break;
    }
  }

  if (isInterpDebug()) {
    std::cerr << "interpExecIf: " << *eif << std::endl;
  }

  return eif;
}

// while ( <expression> ) <statement>
CJExecWhileP
CJavaScript::
interpExecWhile()
{
  CJExecWhileP ewhile(new CJExecWhile);

  ewhile->setLineNum(execLineNum());

  //---

  // while
  if (! interpExecKeyword(CJKeyword::Type::While))
    return CJExecWhileP();

  // ( <expression> )
  if (! interpExecOperator(CJOperator::Type::OpenRBracket)) {
    throwSyntaxError(ewhile, "Missing open bracket for while");
    return CJExecWhileP();
  }

  CJExecExpressionListP exprList = interpExpressionList();

  if (! exprList)
    return CJExecWhileP();

  ewhile->setExprList(exprList);

  if (! interpExecOperator(CJOperator::Type::CloseRBracket)) {
    throwSyntaxError(ewhile, "Missing close bracket for while");
    return CJExecWhileP();
  }

  // <statement>
  CJExecBlockP block = interpExecBlock(CJExecBlock::Type::Iterative);

  if (! block)
    return CJExecWhileP();

  ewhile->setBlock(block);

  //---

  if (isInterpDebug()) {
    std::cerr << "interpExecWhile: " << *ewhile << std::endl;
  }

  return ewhile;
}

// do <statement> while ( <expression> )
CJExecDoP
CJavaScript::
interpExecDo()
{
  CJExecDoP edo(new CJExecDo);

  edo->setLineNum(execLineNum());

  //---

  // do
  if (! interpExecKeyword(CJKeyword::Type::Do))
    return CJExecDoP();

  // <statement>
  CJExecBlockP block = interpExecBlock(CJExecBlock::Type::Iterative);

  if (! block) {
    return CJExecDoP();
  }

  edo->setBlock(block);

  // while
  if (! interpExecKeyword(CJKeyword::Type::While))
    return CJExecDoP();

  // ( <expression> )
  if (! interpExecOperator(CJOperator::Type::OpenRBracket)) {
    throwSyntaxError(edo, "Missing open bracket for do");
    return CJExecDoP();
  }

  CJExecExpressionListP exprList = interpExpressionList();

  if (! exprList) {
    return CJExecDoP();
  }

  edo->setExprList(exprList);

  if (! interpExecOperator(CJOperator::Type::CloseRBracket)) {
    throwSyntaxError(edo, "Missing close bracket for do");
    return CJExecDoP();
  }

  //---

  if (isInterpDebug()) {
    std::cerr << "interpExecDo: " << *edo << std::endl;
  }

  return edo;
}

// switch ( <expression> ) {
//   [ case <expression> : <statementList> ]
//   [ default : <statementList> ]
// }
CJExecSwitchP
CJavaScript::
interpExecSwitch()
{
  CJExecSwitchP eswitch(new CJExecSwitch);

  eswitch->setLineNum(execLineNum());

  //---

  // switch
  if (! interpExecKeyword(CJKeyword::Type::Switch))
    return CJExecSwitchP();

  // ( <expression> )
  if (! interpExecOperator(CJOperator::Type::OpenRBracket)) {
    throwSyntaxError(eswitch, "Missing open bracket for switch");
    return CJExecSwitchP();
  }

  CJExecExpressionListP exprList = interpExpressionList();

  if (! exprList) {
    return CJExecSwitchP();
  }

  eswitch->setExprList(exprList);

  if (! interpExecOperator(CJOperator::Type::CloseRBracket)) {
    throwSyntaxError(eswitch, "Missing close bracket for switch");
    return CJExecSwitchP();
  }

  // {
  if (! interpExecOperator(CJOperator::Type::OpenBrace)) {
    throwSyntaxError(eswitch, "Missing open brace");
    return CJExecSwitchP();
  }

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

        // case <expression> : <statementList>
        // default : <statementList>
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

            if (! eswitch->addDefault(defBlock)) {
              throwSyntaxError(eswitch, "Multiple switch default");
              return CJExecSwitchP();
            }


            defBlock = CJExecBlockP();
          }

          // case <expression> : <statementList>
          // default : <statementList>
          if (keyword->type() == CJKeyword::Type::Case) {
            caseExpr = interpExpression();

            if (! caseExpr) {
              throwSyntaxError(eswitch, "Invalid switch case value");
              return CJExecSwitchP();
            }
          }

          if (! interpExecOperator(CJOperator::Type::Colon)) {
            throwSyntaxError(eswitch, "Missing colon for case expression");
            return CJExecSwitchP();
          }

          if (keyword->type() == CJKeyword::Type::Case) {
            caseBlock = CJExecBlockP(new CJExecBlock(CJExecBlock::Type::Switch));

            caseBlock->setLineNum(keyword->lineNum());
          }
          else {
            defBlock = CJExecBlockP(new CJExecBlock(CJExecBlock::Type::Switch));

            defBlock->setLineNum(keyword->lineNum());
          }

          continue;
        }
        // break
        else if (keyword->type() == CJKeyword::Type::Break) {
          if      (caseBlock)
            caseBlock->addToken(token);
          else if (defBlock)
            defBlock->addToken(token);

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

            if (! eswitch->addDefault(defBlock)) {
              throwSyntaxError(eswitch, "Multiple switch default");
              return CJExecSwitchP();
            }

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
      throwSyntaxError(eswitch, "Missing colon for case expression");
      return CJExecSwitchP();
    }

    execData_->next();
  }

  // }
  if (! interpExecOperator(CJOperator::Type::CloseBrace)) {
    throwSyntaxError(eswitch, "Missing close brace");
    return CJExecSwitchP();
  }

  //---

  if      (caseExpr && caseBlock) {
    caseBlock->interp(this);

    eswitch->addCase(caseExpr, caseBlock);

    caseExpr  = CJExecExpressionP();
    caseBlock = CJExecBlockP();
  }
  else if (defBlock) {
    defBlock->interp(this);

    if (! eswitch->addDefault(defBlock)) {
      throwSyntaxError(eswitch, "Multiple switch default");
      return CJExecSwitchP();
    }

    defBlock = CJExecBlockP();
  }

  //---

  if (isInterpDebug()) {
    std::cerr << "interpExecSwitch: " << *eswitch << std::endl;
  }

  return eswitch;
}

CJExecVarP
CJavaScript::
interpExecVar()
{
  CJExecVarP var(new CJExecVar);

  var->setLineNum(execLineNum());

  //---

  while (! execData_->eof()) {
    // variable name
    if (! canInterpIdentifiers()) {
      throwSyntaxError(var, "Missing identifier");
      return CJExecVarP();
    }

    CJExecIdentifiersP identifiers = interpIdentifiers();

    // variable init value
    if (isExecOperator(CJOperator::Type::Assign)) {
      execData_->next();

      if      (isExecOperator(CJOperator::Type::OpenSBracket)) {
        CJExecArrayP array = interpExecArray();

        if (! array) {
          return CJExecVarP();
        }

        var->addVarValue(identifiers, array);
      }
      else if (isExecOperator(CJOperator::Type::OpenBrace)) {
        if (isInterpDictionary()) {
          CJExecDictionaryP dict = interpExecDictionary();

          if (! dict) {
            return CJExecVarP();
          }

          var->addVarValue(identifiers, dict);
        }
        else {
          CJExecBlockP block = interpExecBlock(CJExecBlock::Type::Sequential);

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
    std::cerr << "interpExecVar: " << *var << std::endl;
  }

  return var;
}

CJExecConstP
CJavaScript::
interpExecConst()
{
  CJExecConstP econst(new CJExecConst);

  econst->setLineNum(execLineNum());

  //---

  // const
  if (! interpExecKeyword(CJKeyword::Type::Const))
    return CJExecConstP();

  // <name> [ = <value> ]
  while (! execData_->eof()) {
    // variable name
    if (! canInterpIdentifiers()) {
      throwSyntaxError(econst, "Missing identifier");
      return CJExecConstP();
    }

    CJExecIdentifiersP identifiers = interpIdentifiers();

    // variable init value
    if (isExecOperator(CJOperator::Type::Assign)) {
      execData_->next();

      if      (isExecOperator(CJOperator::Type::OpenSBracket)) {
        CJExecArrayP array = interpExecArray();

        if (! array) {
          return CJExecConstP();
        }

        econst->addVarValue(identifiers, array);
      }
      else if (isExecOperator(CJOperator::Type::OpenBrace)) {
        if (isInterpDictionary()) {
          CJExecDictionaryP dict = interpExecDictionary();

          if (! dict) {
            return CJExecConstP();
          }

          econst->addVarValue(identifiers, dict);
        }
        else {
          CJExecBlockP block = interpExecBlock(CJExecBlock::Type::Sequential);

          if (! block) {
            return CJExecConstP();
          }

          econst->addVarValue(identifiers, block);
        }
      }
      else {
        CJExecExpressionP expr = interpExpression();

        if (! expr) {
          return CJExecConstP();
        }

        econst->addVarValue(identifiers, expr);
      }
    }
    else {
      econst->addVarValue(identifiers, CJExecExpressionP());
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
    std::cerr << "interpExecConst: " << *econst << std::endl;
  }

  return econst;
}

// with ( <expression> ) <statement>
CJExecWithP
CJavaScript::
interpExecWith()
{
  CJExecWithP ewith(new CJExecWith);

  ewith->setLineNum(execLineNum());

  //---

  // with
  if (! interpExecKeyword(CJKeyword::Type::With))
    return CJExecWithP();

  // ( <expression> )
  if (! interpExecOperator(CJOperator::Type::OpenRBracket)) {
    throwSyntaxError(ewith, "Missing open bracket for with");
    return CJExecWithP();
  }

  CJExecIdentifiersP identifiers = interpIdentifiers();

  if (identifiers) {
    ewith->setIdentifiers(identifiers);
  }
  else {
    CJExecExpressionListP exprList = interpExpressionList();

    if (exprList)
      ewith->setExprList(exprList);
    else
      return CJExecWithP();
  }

  if (! interpExecOperator(CJOperator::Type::CloseRBracket)) {
    throwSyntaxError(ewith, "Missing close bracket for with");
    return CJExecWithP();
  }

  // <statement>
  CJExecBlockP block = interpExecBlock(CJExecBlock::Type::Iterative);

  if (! block)
    return CJExecWithP();

  ewith->setBlock(block);

  //---

  if (isInterpDebug())
    std::cerr << "interpExecWith: " << *ewith << std::endl;

  return ewith;
}

CJExecArrayP
CJavaScript::
interpExecArray()
{
  // [ <ElementList> ]
  // [ ] or [ , ] = [ ]
  // [ , , ] or [ , , ] = [ , , ]
  //
  // <ElementList> :
  //  <AssignmentExpression> [, <AssignmentExpression>]

  CJExecArrayP array(new CJExecArray);

  array->setLineNum(execLineNum());

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

  bool isGet = false, isSet = false;

  CJTokenP token = execData_->token();

  // <name>:<value> or get <name>(<arg>) { <body> }
  if      (token->type() == CJToken::Type::Identifier) {
    std::string name = token->cast<CJIdentifier>()->name();

    isGet = (name == "get");
    isSet = (name == "set");
  }
  // "a":<value>
  else if (token->type() == CJToken::Type::String) {
  }
  // 1:<value>
  else if (token->type() == CJToken::Type::Number) {
  }
  else {
    execData_->setPos(pos);
    return false;
  }

  execData_->next();

  //---

  if      (isExecOperator(CJOperator::Type::Colon)) {
  }
  else if (isGet || isSet) {
    CJTokenP token = execData_->token();

    if (token->type() != CJToken::Type::Identifier) {
      execData_->setPos(pos);
      return false;
    }

    execData_->next();

    //---

    if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
      execData_->setPos(pos);
      return false;
    }

    execData_->next();

    if (isGet) {
      if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
        execData_->setPos(pos);
        return false;
      }

      execData_->next();
    }
    else {
      CJTokenP token = execData_->token();

      if (token->type() != CJToken::Type::Identifier) {
        execData_->setPos(pos);
        return false;
      }

      execData_->next();

      if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
        execData_->setPos(pos);
        return false;
      }

      execData_->next();
    }

    //---

    // { <tokens> }
    if (isExecOperator(CJOperator::Type::CloseBrace)) {
      execData_->setPos(pos);
      return false;
    }

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

      execData_->next();
    }

    if (! isExecOperator(CJOperator::Type::CloseBrace)) {
      execData_->setPos(pos);
      return false;
    }
  }
  else {
    execData_->setPos(pos);
    return false;
  }

  execData_->setPos(pos);

  return true;
}

CJExecDictionaryP
CJavaScript::
interpExecDictionary()
{
  CJExecDictionaryP dict(new CJExecDictionary);

  dict->setLineNum(execLineNum());

  //---

  if (! isExecOperator(CJOperator::Type::OpenBrace))
    return CJExecDictionaryP();

  execData_->next();

  // handle empty dictionary {}
  if (isExecOperator(CJOperator::Type::CloseBrace)) {
    execData_->next();
    return dict;
  }

  //---

  // read key values paits and getters/setters
  //
  //  <key> : <value>
  //  get <key> ( ) { <block> }
  //  set <key> (<arg>) { <block> }
  while (! execData_->eof()) {
    // get key (check for get/set)
    CJValueP key;

    bool isGet = false, isSet = false;

    CJTokenP token = execData_->token();

    if      (token->type() == CJToken::Type::Identifier) {
      std::string name = token->cast<CJIdentifier>()->name();

      isGet = (name == "get");
      isSet = (name == "set");

      key = createStringValue(name);
    }
    else if (token->type() == CJToken::Type::String)
      key = createStringValue(token->cast<CJString>()->text());
    else if (token->type() == CJToken::Type::Number)
      key = std::static_pointer_cast<CJValue>(token);
    else {
      throwSyntaxError(dict, "Invalid key value type");
      return CJExecDictionaryP();
    }

    execData_->next();

    //---

    // <key> : <value>
    if      (interpExecOperator(CJOperator::Type::Colon)) {
      CJExecExpressionP expr = interpExpression();

      if (! expr) {
        throwSyntaxError(dict, "Invalid key value");
        return CJExecDictionaryP();
      }

      dict->addDictionaryValue(key, expr);
    }
    // get <key> ( ) { <block> }
    // set <key> (<arg>) { <block> }
    else if (isGet || isSet) {
      // get key name
      CJTokenP token = execData_->token();

      if (token->type() != CJToken::Type::Identifier) {
        throwSyntaxError(dict, "Missing name for get/set");
        return CJExecDictionaryP();
      }

      std::string propName = token->cast<CJIdentifier>()->name();

      execData_->next();

      //---

      // args
      if (! isExecOperator(CJOperator::Type::OpenRBracket)) {
        throwSyntaxError(dict, "Missing args for get/set");
        return CJExecDictionaryP();
      }

      execData_->next();

      std::string arg;

      if (isGet) {
        if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
          throwSyntaxError(dict, "Missing close bracket for get args");
          return CJExecDictionaryP();
        }

        execData_->next();
      }
      else {
        CJTokenP token = execData_->token();

        if (token->type() != CJToken::Type::Identifier) {
          throwSyntaxError(dict, "Missing arg name for set");
          return CJExecDictionaryP();
        }

        arg = token->cast<CJIdentifier>()->name();

        execData_->next();

        if (! isExecOperator(CJOperator::Type::CloseRBracket)) {
          throwSyntaxError(dict, "Missing close bracket for set arg");
          return CJExecDictionaryP();
        }

        execData_->next();
      }

      //---

      // get function block
      CJExecBlockP block = interpExecBlock(CJExecBlock::Type::Function);

      if (! block)
        return CJExecDictionaryP();

      //---

      CJFunction::Args args;

      if (isSet)
        args.push_back(arg);

      CJFunctionP userFn(new CJFunction(this, propName, args, block));

      userFn->setScope(this);

      //---

      CJGetterSetterP gs = dict->getGetterSetter(propName);

      if (! gs) {
        gs = CJGetterSetterP(new CJGetterSetter(this));

        dict->setGetterSetter(propName, gs);
      }

      if (isGet)
        gs->setGetter(userFn);
      else
        gs->setSetter(userFn);
    }
    else {
      throwSyntaxError(dict, "Missing colon for key value");
      return CJExecDictionaryP();
    }

    // more keys after comma
    if (! isExecOperator(CJOperator::Type::Comma))
      break;

    execData_->next();
  }

  //---

  // ensure close brace
  if (! interpExecOperator(CJOperator::Type::CloseBrace)) {
    throwSyntaxError(dict, "Missing close brace for dictionary");
    return CJExecDictionaryP();
  }

  return dict;
}

CJExecNewP
CJavaScript::
interpExecNew()
{
  CJExecNewP enew = CJExecNewP(new CJExecNew);

  enew->setLineNum(execLineNum());

  //---

  // new
  if (! interpExecKeyword(CJKeyword::Type::New))
    return CJExecNewP();

  // get object name
  CJTokenP token = execData_->token();

  if (token->type() != CJToken::Type::Identifier) {
    throwSyntaxError(enew, "Missing identifier");
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

    if (! interpExecOperator(CJOperator::Type::CloseRBracket)) {
      throwSyntaxError(enew, "Missing close bracket for new");
      return CJExecNewP();
    }
  }

  //---

  if (isInterpDebug())
    std::cerr << "interpExecNew: " << *enew << std::endl;

  return enew;
}

// assert <expression_list>
CJExecAssertP
CJavaScript::
interpExecAssert()
{
  CJExecAssertP eassert(new CJExecAssert);

  eassert->setLineNum(execLineNum());

  //---

  // assert
  if (! interpExecKeyword(CJKeyword::Type::Assert))
    return CJExecAssertP();

  // <expression_list>
  CJExecExpressionListP exprList = interpExpressionList();

  if (! exprList)
    return CJExecAssertP();

  eassert->setExprList(exprList);

  //---

  if (isInterpDebug())
    std::cerr << "interpExecAssert: " << *eassert << std::endl;

  return eassert;
}

// break [<label>]
CJExecBreakP
CJavaScript::
interpExecBreak()
{
  CJExecBreakP ebreak(new CJExecBreak);

  ebreak->setLineNum(execLineNum());

  //---

  // break
  if (! interpExecKeyword(CJKeyword::Type::Break))
    return CJExecBreakP();

  // [ <label> ]
  if (canInterpIdentifiers()) {
    CJExecIdentifiersP identifiers = interpIdentifiers();

    if (! identifiers)
      return CJExecBreakP();

    ebreak->setIdentifiers(identifiers);
  }

  //---

  if (isInterpDebug())
    std::cerr << "interpExecBreak: " << *ebreak << std::endl;

  return ebreak;
}

// continue [<label>]
CJExecContinueP
CJavaScript::
interpExecContinue()
{
  CJExecContinueP econt(new CJExecContinue);

  econt->setLineNum(execLineNum());

  //---

  // continue
  if (! interpExecKeyword(CJKeyword::Type::Continue))
    return CJExecContinueP();

  // [<label>]
  if (canInterpIdentifiers()) {
    CJExecIdentifiersP identifiers = interpIdentifiers();

    if (! identifiers)
      return CJExecContinueP();

    econt->setIdentifiers(identifiers);
  }

  //---

  if (isInterpDebug())
    std::cerr << "interpExecContinue: " << *econt << std::endl;

  return econt;
}

// return [<expression>]
CJExecReturnP
CJavaScript::
interpExecReturn()
{
  CJExecReturnP eret(new CJExecReturn);

  eret->setLineNum(execLineNum());

  //---

  // return
  if (! interpExecKeyword(CJKeyword::Type::Return))
    return CJExecReturnP();

  // [<expression>]
  CJExecExpressionP expr = interpExpression();

  if (expr)
    eret->setExpr(expr);

  //---

  if (isInterpDebug())
    std::cerr << "interpExecReturn: " << *eret << std::endl;

  return eret;
}

// try <block> [catch (<parameter>) <block>] [finally <block>]
CJExecTryP
CJavaScript::
interpExecTry()
{
  CJExecTryP etry(new CJExecTry);

  etry->setLineNum(execLineNum());

  //---

  // try
  if (! interpExecKeyword(CJKeyword::Type::Try))
    return CJExecTryP();

  // <block>
  CJExecBlockP block1 = interpExecBlock(CJExecBlock::Type::Sequential);

  if (! block1) {
    return CJExecTryP();
  }

  etry->setTryBlock(block1);

  if (! isExecKeyword(CJKeyword::Type::Catch) &&
      ! isExecKeyword(CJKeyword::Type::Finally)) {
    throwSyntaxError(etry, "Missing catch or finally for try");
    return CJExecTryP();
  }

  // catch (<parameter>) <block>
  if (interpExecKeyword(CJKeyword::Type::Catch)) {
    // (<parameter>)
    if (! interpExecOperator(CJOperator::Type::OpenRBracket)) {
      throwSyntaxError(etry, "Missing open bracket for catch");
      return CJExecTryP();
    }

    CJExecIdentifiersP identifiers = interpIdentifiers();

    if (! identifiers) {
      return CJExecTryP();
    }

    etry->setCatchIdentifiers(identifiers);

    if (! interpExecOperator(CJOperator::Type::CloseRBracket)) {
      throwSyntaxError(etry, "Missing close bracket for catch");
      return CJExecTryP();
    }

    // <block>
    CJExecBlockP block2 = interpExecBlock(CJExecBlock::Type::Sequential);

    if (! block2) {
      return CJExecTryP();
    }

    etry->setCatchBlock(block2);
  }

  // finally <block>
  if (interpExecKeyword(CJKeyword::Type::Finally)) {
    // <block>
    CJExecBlockP block3 = interpExecBlock(CJExecBlock::Type::Sequential);

    if (! block3) {
      return CJExecTryP();
    }

    etry->setFinallyBlock(block3);
  }

  //---

  if (isInterpDebug())
    std::cerr << "interpExecTry: " << *etry << std::endl;

  return etry;
}

// throw <expression>
CJExecThrowP
CJavaScript::
interpExecThrow()
{
  CJExecThrowP ethrow(new CJExecThrow);

  ethrow->setLineNum(execLineNum());

  //---

  // throw
  if (! interpExecKeyword(CJKeyword::Type::Throw))
    return CJExecThrowP();

  // <expression>
  CJExecExpressionP expr = interpExpression();

  if (! expr)
    return CJExecThrowP();

  ethrow->setExpression(expr);

  //---

  if (isInterpDebug())
    std::cerr << "interpExecThrow: " << *ethrow << std::endl;

  return ethrow;
}

// function [<identifier>] ( <parameters> ) { <body> }
CJFunctionP
CJavaScript::
interpUserFunction(bool named)
{
  // function
  if (! interpExecKeyword(CJKeyword::Type::Function))
    return CJFunctionP();

  // [<identifier>]
  std::string name;

  CJTokenP token = execData_->token();

  if (token->type() == CJToken::Type::Identifier) {
    name = token->cast<CJIdentifier>()->name();

    execData_->next();
  }
  else {
    if (named) {
      throwSyntaxError(token, "Missing identifier");
      return CJFunctionP();
    }
  }

  CJFunctionP userFn(new CJFunction(this, name));

  //---

  // ( <parameters> )
  if (! interpExecOperator(CJOperator::Type::OpenRBracket)) {
    throwSyntaxError(token, "Missing open bracket for function");
    return CJFunctionP();
  }

  std::vector<std::string> args;

  token = execData_->token();

  while (token->type() == CJToken::Type::Identifier) {
    std::string arg = token->cast<CJIdentifier>()->name();

    args.push_back(arg);

    execData_->next();

    if (isExecOperator(CJOperator::Type::CloseRBracket))
      break;

    if (! interpExecOperator(CJOperator::Type::Comma)) {
      throwSyntaxError(token, "Missing comma for function args");
      return CJFunctionP();
    }

    token = execData_->token();

    if (token->type() != CJToken::Type::Identifier) {
      throwSyntaxError(token, "Missing arg after comma for function args");
      return CJFunctionP();
    }
  }

  if (! interpExecOperator(CJOperator::Type::CloseRBracket)) {
    throwSyntaxError(token, "Missing close bracket for function");
    return CJFunctionP();
  }

  userFn->setArgs(args);

  //---

//printUserFunctions("interpUserFunction");
  userFn->setScope(this, currentUserFunction());

  // { <block> }
  if (! isExecOperator(CJOperator::Type::OpenBrace)) {
    throwSyntaxError(userFn, "Missing open brace for function");
    return CJFunctionP();
  }

  pushUserFunction(userFn);

  CJExecBlockP block = interpExecBlock(CJExecBlock::Type::Function);

  if (block)
    userFn->setBlock(block);

  popUserFunction();

  if (! block)
    return CJFunctionP();

  //---

  if (isInterpDebug())
    std::cerr << "interpUserFunction: " << *userFn << std::endl;

  return userFn;
}

bool
CJavaScript::
isExecLabel() const
{
  int pos = execData_->pos();

  CJTokenP token = execData_->token();

  CJToken::Type type = token->type();

  if (type != CJToken::Type::Identifier)
    return false;

  execData_->next();

  if (! isExecOperator(CJOperator::Type::Colon)) {
    execData_->setPos(pos);
    return false;
  }

  execData_->setPos(pos);

  return true;
}

// <identifier> : <statement>
CJExecLabelP
CJavaScript::
interpExecLabel()
{
  CJExecLabelP label = CJExecLabelP(new CJExecLabel);

  label->setLineNum(execLineNum());

  //---

  CJTokenP token = execData_->token();

  if (token->type() != CJToken::Type::Identifier)
    return label;

  label->setName(token->cast<CJIdentifier>()->name());

  execData_->next();

  if (! isExecOperator(CJOperator::Type::Colon))
    return label;

  execData_->next();

  if (isInterpDebug()) {
    std::cerr << "interpExecLabel: " << *label << std::endl;
  }

  return label;
}

CJExecExpressionListP
CJavaScript::
interpExpressionList()
{
  CJExecExpressionListP exprList(new CJExecExpressionList);

  exprList->setLineNum(execLineNum());

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

  expr->setLineNum(execLineNum());

  //---

  while (! execData_->eof()) {
    CJTokenP token = execData_->token();

    if      (canInterpIdentifiers()) {
      CJToken::Type lastType = expr->lastTokenType();

      if (lastType == CJToken::Type::Identifiers ||
          lastType == CJToken::Type::Function) {
        std::stringstream ss; ss << *token;
        throwSyntaxError(execData_->token(), "Syntax error at identifier '" + ss.str() + "'");
        return CJExecExpressionP();
      }

      bool allowIncrDecr = true;

      CJExecIndexExpressionP iexpr;

      CJExecIdentifiersP identifiers = interpIdentifiers();

      while (isExecOperator(CJOperator::Type::OpenSBracket)) {
        execData_->next();

        CJExecIndexExpressionP iexpr1(new CJExecIndexExpression);

        if (identifiers) {
          iexpr1->setLineNum(identifiers->lineNum());

          iexpr1->setIdentifiers(identifiers);

          identifiers = 0;
        }
        else {
          iexpr1->setLineNum(iexpr->lineNum());

          iexpr1->setExpression(iexpr);
        }

        iexpr = iexpr1;

        //----

        CJExecExpressionP expr1 = interpExpression();

        if (expr1)
          iexpr->setIndexExpression(expr1);

        if (! interpExecOperator(CJOperator::Type::CloseSBracket)) {
          throwSyntaxError(execData_->token(), "Missing close square bracket");
          return CJExecExpressionP();
        }
      }

      if      (isExecOperator(CJOperator::Type::OpenRBracket)) {
        CJExecFunctionP function = interpFunction();

        if (! function)
          return CJExecExpressionP();

        if (iexpr) {
          if (isInterpDebug()) {
            std::cerr << "interpIndexExpression: " << *iexpr << std::endl;
          }

          function->setExpression(iexpr);

          function->setLineNum(iexpr->lineNum());
        }
        else {
          function->setIdentifiers(identifiers);

          function->setLineNum(identifiers->lineNum());
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

          if (! function1)
            return CJExecExpressionP();

          function1->setIdentifiers(identifiers);

          if (isInterpDebug()) {
            std::cerr << "interpFunction: " << *function1 << std::endl;
          }

          //---

          function->addFunction(function1);
        }
#endif

        allowIncrDecr = false;
      }

      if (allowIncrDecr) {
        if (isExecOperator(CJOperator::Type::Increment) ||
            isExecOperator(CJOperator::Type::Decrement)) {
          CJTokenP token1 = execData_->token();

          execData_->next();

          CJExecIncrDecrExpressionP incrDecr(new CJExecIncrDecrExpression);

          incrDecr->setLineNum(token1->lineNum());

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
      CJToken::Type lastType = expr->lastTokenType();

      if (lastType == CJToken::Type::Number) {
        std::stringstream ss; ss << *token;
        throwSyntaxError(execData_->token(), "Syntax error at value '" + ss.str() + "'");
        return CJExecExpressionP();
      }

      execData_->next();

      // TODO: needed
      if (token->isProtoValue() && isExecOperator(CJOperator::Type::Scope)) {
        execData_->next();

        CJExecIdentifiersP identifiers = interpIdentifiers();

        CJExecFunctionP function = interpFunction();

        if (! function)
          return CJExecExpressionP();

        expr->addToken(token);

        function->setExpression (expr);
        function->setIdentifiers(identifiers);

        if (isInterpDebug()) {
          std::cerr << "interpFunction: " << *function << std::endl;
        }

        //---

        expr = CJExecExpressionP(new CJExecExpression);

        expr->setLineNum(function->lineNum());

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

        assign->setLineNum(op->lineNum());

        assign->setLExpression(expr);
        assign->setOperator   (op);

        CJExecExpressionP expr1 = interpExpression();

        if (! expr1) {
          throwSyntaxError(expr, "invalid assignment rhs expression");
          return CJExecExpressionP();
        }

        assign->setRExpression(expr1);

        return std::static_pointer_cast<CJExecExpression>(assign);
      }
      else if (opType == CJOperator::Type::Increment ||
               opType == CJOperator::Type::Decrement) {
        execData_->next();

        if      (canInterpIdentifiers()) {
          CJExecIdentifiersP identifiers = interpIdentifiers();

          CJExecIncrDecrExpressionP incrDecr(new CJExecIncrDecrExpression);

          incrDecr->setLineNum(identifiers->lineNum());

          incrDecr->setIdentifiers(identifiers);
          incrDecr->setOperator   (op);
          incrDecr->setPost       (false);

          expr->addToken(std::static_pointer_cast<CJToken>(incrDecr));
        }
#if 0
        else if (isExecKeyword(CJKeyword::Type::This)) {
          CJExecThisP ethis = interpExecThis();

          ethis->setIncrDecr(op, /*postOp*/false);

          // TODO: disallow pre and post op
          execData_->addEToken(std::static_pointer_cast<CJToken>(ethis));
        }
#endif
        else {
          throwSyntaxError(expr, "invalid increment/decrement rhs");
          return CJExecExpressionP();
        }
      }
      else if (opType == CJOperator::Type::Question) {
        execData_->next();

        CJExecQuestionP equestion = interpExecQuestion(expr);

        if (! equestion) {
          throwSyntaxError(expr, "Interp failed for '?'");
          break;
        }

        expr = CJExecExpressionP(new CJExecExpression);

        expr->setLineNum(equestion->lineNum());

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
        if (expr->lastToken() && expr->lastTokenType() != CJToken::Type::Operator) {
          throwTypeError(expr, "Invalid function operator");
          return CJExecExpressionP();
        }

        execData_->next();

        CJExecExpressionListP expr1 = interpExpressionList();

        if (expr1)
          expr->addToken(std::static_pointer_cast<CJToken>(expr1));

        if (! interpExecOperator(CJOperator::Type::CloseRBracket)) {
          throwSyntaxError(expr, "Missing close round bracket");
          return CJExecExpressionP();
        }

        // check for call of bracketed expression
        if (isExecOperator(CJOperator::Type::OpenRBracket)) {
          CJExecFunctionP function = interpFunction();

          if (! function)
            return CJExecExpressionP();

          if (isInterpDebug())
            std::cerr << "interpFunction: " << *function << std::endl;

          CJExecExpressionP expr1 = expr;

          expr = CJExecExpressionP(new CJExecExpression);

          expr->setLineNum(execLineNum());

          function->setLineNum(expr1->lineNum());

          function->setExpression(expr1);

          expr->addToken(CJTokenP(function));

          //allowIncrDecr = false;
        }
      }
      else if (opType == CJOperator::Type::CloseRBracket) {
        return expr;
      }
      else if (opType == CJOperator::Type::OpenSBracket) {
        if (! expr->hasTokens()) {
          CJExecArrayP array = interpExecArray();

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

          if (! function)
            return CJExecExpressionP();

          expr->addToken(CJTokenP(array));

          function->setExpression (expr);
          function->setIdentifiers(identifiers);

          if (isInterpDebug()) {
            std::cerr << "interpFunction: " << *function << std::endl;
          }

          //---

          expr = CJExecExpressionP(new CJExecExpression);

          expr->setLineNum(function->lineNum());

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
          CJExecDictionaryP dict = interpExecDictionary();

          if (! dict) {
            return CJExecExpressionP();
          }

#if 0
          if (isExecOperator(CJOperator::Type::Scope)) {
            execData_->next();

            CJExecIdentifiersP identifiers = interpIdentifiers();

            CJExecFunctionP function = interpFunction();

            if (! function)
              return CJExecExpressionP();

            expr->addToken(CJTokenP(dict));

            function->setExpression (expr);
            function->setIdentifiers(identifiers);

            if (isInterpDebug()) {
              std::cerr << "interpFunction: " << *function << std::endl;
            }

            //---

            expr = CJExecExpressionP(new CJExecExpression);

            expr->setLineNum(function->lineNum());

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
          CJExecBlockP block = interpExecBlock(CJExecBlock::Type::Iterative);

          if (! block) {
            return CJExecExpressionP();
          }

          expr->addToken(block);

          break;
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
        CJExecNewP enew = interpExecNew();

        if (! enew) {
          throwSyntaxError(expr, "Interp failed for 'new'");
          break;
        }

        expr->addToken(std::static_pointer_cast<CJToken>(enew));
      }
      else if (keyword->type() == CJKeyword::Type::Function) {
        CJFunctionP efunction = interpUserFunction(/*named*/false);

        if (! efunction) {
          throwSyntaxError(expr, "Interp failed for 'function'");
          break;
        }

        CJValueP value(efunction);

        setProperty(efunction->name(), value);

        expr->addToken(value);
      }
      else {
        throwSyntaxError(keyword, "Interp failed at keyword: " + keyword->name());
        break;
      }
    }
    else {
      std::stringstream ss; ss << *token;
      throwSyntaxError(expr, "Interp failed at token: " + ss.str());
      return CJExecExpressionP();
    }
  }

  //---

  if (isInterpDebug())
    std::cerr << "interpExpression: " << *expr << std::endl;

  return expr;
}

CJExecThisP
CJavaScript::
interpExecThis()
{
  if (! interpExecKeyword(CJKeyword::Type::This)) {
    throwSyntaxError(execData_->token(), "Missing keyword this");
    return CJExecThisP();
  }

  CJExecThisP ethis(new CJExecThis);

  ethis->setLineNum(execLineNum());

  //---

  if      (isExecOperator(CJOperator::Type::Scope)) {
    execData_->next();

    CJExecIdentifiersP identifiers = interpIdentifiers();

    ethis->setIdentifiers(identifiers);
  }
  else if (isExecOperator(CJOperator::Type::OpenSBracket)) {
    CJExecIndexExpressionP iexpr = interpIndexExpression();

    ethis->setIndexExpression(iexpr);
  }

  if      (isExecOperator(CJOperator::Type::Assign)) {
    execData_->next();

    CJExecExpressionP expr = interpExpression();

    if (! expr) {
      throwSyntaxError(execData_->token(), "Missing expression for assign");
      return CJExecThisP();
    }

    ethis->setAssignExpression(expr);
  }
  else if (isExecOperator(CJOperator::Type::Increment) ||
           isExecOperator(CJOperator::Type::Decrement)) {
    CJTokenP token1 = execData_->token();

    execData_->next();

    CJOperatorP op1 = std::static_pointer_cast<CJOperator>(token1);

    ethis->setIncrDecr(op1, /*postOp*/true);
  }

  //---

  if (isInterpDebug())
    std::cerr << "interpExecThis: " << *ethis << std::endl;

  return ethis;
}

CJExecIndexExpressionP
CJavaScript::
interpIndexExpression()
{
  if (! interpExecOperator(CJOperator::Type::OpenSBracket)) {
    throwSyntaxError(execData_->token(), "Missing open square bracket");
    return CJExecIndexExpressionP();
  }

  CJExecIndexExpressionP iexpr(new CJExecIndexExpression);

  iexpr->setLineNum(execLineNum());

  //---

  CJExecExpressionP expr = interpExpression();

  if (expr)
    iexpr->setIndexExpression(expr);

  if (! interpExecOperator(CJOperator::Type::CloseSBracket)) {
    throwSyntaxError(execData_->token(), "Missing close square bracket");
    return CJExecIndexExpressionP();
  }

  if (isInterpDebug()) {
    std::cerr << "interpIndexExpression: " << *iexpr << std::endl;
  }

  return iexpr;
}

bool
CJavaScript::
canInterpIdentifiers() const
{
  if (execData_->eof())
    return false;

  CJTokenP token = execData_->token();

  if (token->type() == CJToken::Type::Identifier)
    return true;

  if (isExecKeyword(CJKeyword::Type::This))
    return true;

  return false;
}

CJExecIdentifiersP
CJavaScript::
interpIdentifiers()
{
  CJExecIdentifiersP identifiers(new CJExecIdentifiers);

  identifiers->setLineNum(execLineNum());

  //---

  if (! canInterpIdentifiers())
    return CJExecIdentifiersP();

  CJTokenP token = execData_->token();

  if      (token->type() == CJToken::Type::Identifier)
    identifiers->addIdentifier(token->cast<CJIdentifier>());
  else if (isExecKeyword(CJKeyword::Type::This))
    identifiers->setIsThis(true);

  execData_->next();

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
interpExecBlock(CJExecBlock::Type type)
{
  CJExecBlockP block(new CJExecBlock(type));

  block->setLineNum(execLineNum());

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

    if (! interpExecOperator(CJOperator::Type::CloseBrace)) {
      throwSyntaxError(block, "Missing close brace");
      return CJExecBlockP();
    }
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
    std::cerr << "interpExecBlock: " << *block << std::endl;
  }

  return block;
}

bool
CJavaScript::
interpExecKeyword(CJKeyword::Type type)
{
  if (! isExecKeyword(type))
    return false;

  execData_->next();

  return true;
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
interpExecOperator(CJOperator::Type opType)
{
  if (! isExecOperator(opType))
    return false;

  execData_->next();

  return true;
}

bool
CJavaScript::
isExecOperator(CJOperator::Type opType) const
{
  if (execData_->eof())
    return false;

  CJTokenP token = execData_->token();

  return isOperator(token, opType);
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

int
CJavaScript::
execLineNum() const
{
  if (execData_->eof())
    return -1;

  return execData_->token()->lineNum();
}

CJFunctionBaseP
CJavaScript::
valueToFunction(CJValueP value) const
{
  CJFunctionBaseP fn;

  if      (value->isFunction()) {
    fn = std::static_pointer_cast<CJFunctionBase>(value);
  }
  else if (value->type() == CJToken::Type::UserObject) {
    CJUserObjectP userObj = std::static_pointer_cast<CJUserObject>(value);

    fn = std::static_pointer_cast<CJFunctionBase>(userObj->userFn());
  }
  else if (value->type() == CJToken::Type::Dictionary) {
    CJDictionaryP dict = std::static_pointer_cast<CJDictionary>(value);

    CJValueP cvalue = dict->getProperty(const_cast<CJavaScript *>(this), "constructor");

    if (cvalue)
      fn = valueToFunction(cvalue);
  }

  return fn;
}

COptInt
CJavaScript::
cmp(CJValueP value1, CJValueP value2)
{
  if      (value1->type() == CJValue::Type::String && value2->type() == CJValue::Type::String) {
    std::string s1 = value1->toString();
    std::string s2 = value2->toString();

    if (s1 < s2) return COptInt(-1);
    if (s1 > s2) return COptInt( 1);

    return COptInt(0);
  }
  else if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
    double r1 = value1->toReal();
    double r2 = value2->toReal();

    if (CJUtil::isNaN(r1) || CJUtil::isNaN(r2))
      return COptInt();

    if (CJUtil::isPosInf(r1) && CJUtil::isPosInf(r2)) return COptInt(0);
    if (CJUtil::isNegInf(r1) && CJUtil::isNegInf(r2)) return COptInt(0);

    if (CJUtil::isPosInf(r1) || CJUtil::isNegInf(r2)) return COptInt( 1);
    if (CJUtil::isNegInf(r1) || CJUtil::isPosInf(r2)) return COptInt(-1);

    if (r1 < r2) return COptInt(-1);
    if (r1 > r2) return COptInt( 1);

    return COptInt(0);
  }
  else if (value1->type() == CJValue::Type::Undefined ||
           value2->type() == CJValue::Type::Undefined) {
    if (value1->type() == value2->type())
      return COptInt(0);

    if (value1->type() == CJValue::Type::Null || value2->type() == CJValue::Type::Null)
      return COptInt(0);

    return COptInt();
  }
  else if (value1->type() == CJValue::Type::Null ||
           value2->type() == CJValue::Type::Null) {
    if (value1->type() == value2->type())
      return COptInt(0);

    if (value1->type() == CJValue::Type::Undefined || value2->type() == CJValue::Type::Undefined)
      return COptInt(0);

    return COptInt();
  }
  else if (value1->isBoolType() || value2->isBoolType()) {
    bool b1 = value1->toBoolean();
    bool b2 = value2->toBoolean();

    if (b1 < b2) return COptInt(-1);
    if (b1 > b2) return COptInt( 1);

    return COptInt(0);
  }
  else if (value1->type() == CJValue::Type::Number || value2->type() == CJValue::Type::Number) {
    double r1 = value1->toReal();
    double r2 = value2->toReal();

    if (r1 < r2) return COptInt(-1);
    if (r1 > r2) return COptInt( 1);

    return COptInt(0);
  }
  else if (value1->type() == CJValue::Type::String || value2->type() == CJValue::Type::String) {
    std::string s1 = value1->toString();
    std::string s2 = value2->toString();

    if (s1 < s2) return COptInt(-1);
    if (s1 > s2) return COptInt( 1);

    return COptInt(0);
  }
  else if (value1->type() == CJValue::Type::Object && value2->type() == CJValue::Type::Object) {
    CJObjP obj1 = std::static_pointer_cast<CJObj>(value1);
    CJObjP obj2 = std::static_pointer_cast<CJObj>(value2);

    return COptInt(obj1->cmp(obj2));
  }
  else {
    double r1 = value1->toReal();
    double r2 = value2->toReal();

    if (r1 < r2) return COptInt(-1);
    if (r1 > r2) return COptInt( 1);

    return COptInt(0);
  }
}

CJValueP
CJavaScript::
execBinaryOp(CJOperator::Type op, CJValueP value1, CJValueP value2)
{
  if (isExprDebug()) {
    if (value1) std::cerr << *value1; else std::cerr << "<null>";
    std::cerr << CJOperator::typeName(op) << " ";
    if (value2) std::cerr << *value2; else std::cerr << "<null>";
    std::cerr << std::endl;
  }

  if (! value1 || ! value2) {
    if      (op == CJOperator::Type::StrictEqual) {
      return createBoolValue(value1 == value2);
    }
    else if (op == CJOperator::Type::StrictNotEqual) {
      return createBoolValue(value1 != value2);
    }
    else if (op == CJOperator::Type::LogicalOr) {
      return (value1 ? value1 : value2);
    }
    else if (op == CJOperator::Type::LogicalAnd) {
      return CJValueP();
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

        return createNumberValue(r1 + r2);
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

        return createNumberValue(r1 + r2);
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

        if (CJUtil::isNaN(r1) || CJUtil::isNaN(r2))
          return createNumberValue(CJUtil::getNaN());
        if (CJUtil::isInf(r1) && CJUtil::isInf(r2))
          return createNumberValue(CJUtil::getNaN());

        if (CJUtil::isPosInf(r1))
          return createNumberValue(CJUtil::getPosInf());
        if (CJUtil::isNegInf(r1))
          return createNumberValue(CJUtil::getNegInf());
        if (CJUtil::isPosInf(r2))
          return createNumberValue(CJUtil::getNegInf());
        if (CJUtil::isNegInf(r2))
          return createNumberValue(CJUtil::getPosInf());

        return createNumberValue(r1 - r2);
      }

      break;
    case CJOperator::Type::Times:
      if (value1->type() == CJValue::Type::Number ||
          value2->type() == CJValue::Type::Number) {
        double r1 = value1->toReal();
        double r2 = value2->toReal();

        return createNumberValue(r1 * r2);
      }

      break;
    case CJOperator::Type::Divide:
      if (value1->type() == CJValue::Type::Number ||
          value2->type() == CJValue::Type::Number) {
        double r1 = value1->toReal();
        double r2 = value2->toReal();

        if      (r2 != 0)
          return createNumberValue(r1 / r2);
        else if (r1 >= 0)
          return createNumberValue(CJUtil::getPosInf());
        else
          return createNumberValue(CJUtil::getNegInf());
      }

      break;
    case CJOperator::Type::Modulus:
      if (value1->type() == CJValue::Type::Number ||
          value2->type() == CJValue::Type::Number) {
        double r1 = value1->toReal();
        double r2 = value2->toReal();

        return createNumberValue(CJUtil::realModulus(r1, r2));
      }

      break;

    case CJOperator::Type::Less:
    case CJOperator::Type::Greater:
    case CJOperator::Type::LessEqual:
    case CJOperator::Type::GreaterEqual:
    case CJOperator::Type::Equal:
    case CJOperator::Type::NotEqual: {
      COptInt d = cmp(value1, value2);

      if (! d.isValid())
        return createBoolValue(false);

      if      (op == CJOperator::Type::Less)
        return createBoolValue(d <  0);
      else if (op == CJOperator::Type::Greater)
        return createBoolValue(d >  0);
      else if (op == CJOperator::Type::LessEqual)
        return createBoolValue(d <= 0);
      else if (op == CJOperator::Type::GreaterEqual)
        return createBoolValue(d >= 0);
      else if (op == CJOperator::Type::Equal)
        return createBoolValue(d == 0);
      else if (op == CJOperator::Type::NotEqual)
        return createBoolValue(d != 0);
      else
        assert(false);
    }

    case CJOperator::Type::StrictEqual:
    case CJOperator::Type::StrictNotEqual: {
      COptInt d = cmp(value1, value2);

      if (! d.isValid())
        return createBoolValue(false);

      if (op == CJOperator::Type::StrictEqual)
        return createBoolValue(value1->type() == value2->type() && d.getValue() == 0);
      else
        return createBoolValue(value1->type() != value2->type() || d.getValue() != 0);
    }

    case CJOperator::Type::LogicalAnd: {
#if 0
      bool i1 = value1->toBoolean();

      bool res = false;

      if (i1) {
        bool i2 = value2->toBoolean();

        res = (i1 && i2);
      }

      return createBoolValue(res);
#else
      bool i1 = value1->toBoolean();

      if (i1)
        return value2;
      else
        return value1;
#endif
    }

    case CJOperator::Type::LogicalOr: {
#if 0
      bool i1 = value1->toBoolean();

      bool res = true;

      if (! i1) {
        bool i2 = value2->toBoolean();

        res = (i1 || i2);
      }

      return createBoolValue(res);
#else
      bool i1 = value1->toBoolean();

      if (i1)
        return value1;
      else
        return value2;
#endif
    }

    case CJOperator::Type::BitwiseAnd:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        long res = (i1 & i2);

        return createNumberValue(res);
      }

      break;
    case CJOperator::Type::BitwiseOr:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        long res = (i1 | i2);

        return createNumberValue(res);
      }

      break;
    case CJOperator::Type::BitwiseXor:
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        long res = (i1 ^ i2);

        return createNumberValue(res);
      }

      break;

    case CJOperator::Type::LeftShift: {
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        long res = (i1 << i2);

        return createNumberValue(res);
      }

      break;
    }
    case CJOperator::Type::RightShift: {
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        long i1 = value1->toInteger();
        long i2 = value2->toInteger();

        long res = (i1 >> i2);

        return createNumberValue(res);
      }

      break;
    }
    case CJOperator::Type::UnsignedRightShift: {
      if (value1->type() == CJValue::Type::Number && value2->type() == CJValue::Type::Number) {
        ulong i1 = value1->toInteger();
        long  i2 = value2->toInteger();

        long res = (i1 >> i2);

        return createNumberValue(res);
      }

      break;
    }

    case CJOperator::Type::InstanceOf: {
      CJObjTypeP valueType1, valueType2;

      if      (value1->type() == CJToken::Type::Object)
        valueType1 = value1->cast<CJObj>()->valueType();
      else if (value1->isFunction()) {
        CJFunctionBaseP fn = std::static_pointer_cast<CJFunctionBase>(value1);

        if (fn->type() == CJFunctionBase::Type::ObjType) {
          CJTypeFunctionP typeFn = std::static_pointer_cast<CJTypeFunction>(value1);

          valueType1 = typeFn->objectType();
        }
      }
      else
        valueType1 = value1->valueType();

      if      (value2->type() == CJToken::Type::Object)
        valueType2 = value2->cast<CJObj>()->valueType();
      else if (value2->isFunction()) {
        CJFunctionBaseP fn = std::static_pointer_cast<CJFunctionBase>(value2);

        if (fn->type() == CJFunctionBase::Type::ObjType) {
          CJTypeFunctionP typeFn = std::static_pointer_cast<CJTypeFunction>(value2);

          valueType2 = typeFn->objectType();
        }
      }
      else
        valueType2 = value2->valueType();

      return createBoolValue(valueType1->isInstanceOf(valueType2));
    }

    case CJOperator::Type::In: {
      if      (value2->type() == CJToken::Type::Array) {
        CJArrayP array2 = std::static_pointer_cast<CJArray>(value2);

        return createBoolValue(array2->hasValue(value1));
      }
      else if (value2->hasIndex()) {
        long ind = value1->toInteger();

        return createBoolValue(value2->hasIndexValue(ind));
      }
      else if (value2->hasProperty()) {
        std::string ind = value1->toString();

        return createBoolValue(value2->hasPropertyValue(ind));
      }
      else
        return createFalseValue();
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
  if (isExprDebug()) {
    std::cerr << CJOperator::typeName(op) << " ";
    if (value) std::cerr << *value; else std::cerr << "<null>";
    std::cerr << std::endl;
  }

  if (! value) {
    if      (op == CJOperator::Type::TypeOf)
      return createUndefinedValue();
    else if (op == CJOperator::Type::Void)
      return createUndefinedValue();
    else if (op == CJOperator::Type::Delete)
      return createBoolValue(true);
    else if (op == CJOperator::Type::LogicalNot)
      return createBoolValue(true);
    else
      return CJValueP();
  }

  switch (op) {
    case CJOperator::Type::UnaryPlus: {
      double r = value->toReal();

      return createNumberValue(r);
    }
    case CJOperator::Type::UnaryMinus: {
      double r = value->toReal();

      return createNumberValue(-r);
    }

    case CJOperator::Type::Increment:
      if (value->type() == CJValue::Type::Number) {
        double r = value->cast<CJNumber>()->real();

        return createNumberValue(r + 1);
      }

      break;
    case CJOperator::Type::Decrement:
      if (value->type() == CJValue::Type::Number) {
        double r = value->cast<CJNumber>()->real();

        return createNumberValue(r - 1);
      }

      break;

    case CJOperator::Type::LogicalNot: {
      if (value->type() == CJValue::Type::Object)
        return createBoolValue(false);

      bool i = value->toBoolean();

      bool res = ! i;

      return createBoolValue(res);
    }

    case CJOperator::Type::BitwiseNot:
      if (value->type() == CJValue::Type::Number) {
        long i = value->toInteger();

        long res = ~ i;

        return createNumberValue(res);
      }

      break;

    case CJOperator::Type::TypeOf: {
      std::string res;

      if      (value->isFunction())
        res = "function";
      else if (value->valueType()->type() == CJToken::Type::Dictionary)
        res = "object";
      else if (value->valueType()->type() == CJToken::Type::Array)
        res = "object";
      else if (value->valueType()->type() == CJToken::Type::Number)
        res = "number";
      else if (value->valueType()->type() == CJToken::Type::String)
        res = "string";
      else if (value->valueType()->type() == CJToken::Type::True ||
               value->valueType()->type() == CJToken::Type::False)
        res = "boolean";
      else if (value->valueType()->type() == CJToken::Type::Null)
        res = "object";
      else if (value->valueType()->type() == CJToken::Type::Undefined)
        res = "undefined";
      else
        res = value->valueType()->name();

      return createStringValue(res);
    }

    case CJOperator::Type::Delete: {
      if (value->type() == CJToken::Type::TokenValue) {
        CJTokenValueP tokenValue = std::static_pointer_cast<CJTokenValue>(value);

        bool b = deleteToken(tokenValue->tokens());

        return createBoolValue(b);
      }
      else
        return value;
    }

    case CJOperator::Type::Void: {
      return createUndefinedValue();
    }

    default:
      assert(false);
      break;
  }

  return CJValueP();
}

std::string
CJavaScript::
getIdentifier(CStrParse &parse, bool allowUnary) const
{
  int pos = parse.getPos();

  std::string name;

  if (allowUnary && parse.isChar('-'))
    name += parse.readChar();

  while (! parse.eof() && (parse.isAlnum() || parse.isOneOf("_$"))) {
    name += parse.readChar();
  }

  parse.setPos(pos);

  return name;
}

void
CJavaScript::
readIdentifier(CStrParse &parse)
{
  std::string name;

  while (! parse.eof() && (parse.isAlnum() || parse.isOneOf("_$"))) {
    name += parse.readChar();
  }

  CJTokenP token;

  if      (name == "null")
    token = CJTokenP(createNullValue());
  else if (name == "true")
    token = CJTokenP(createTrueValue());
  else if (name == "false")
    token = CJTokenP(createFalseValue());
  else if (name == "undefined")
    token = CJTokenP(createUndefinedValue());
  else {
    auto k = isKeyword(name);

    if (k.first)
      token = CJTokenP(new CJKeyword(k.second));
    else
      token = CJTokenP(new CJIdentifier(name));
  }

  token->setLineNum(parse.lineNum());

  tokens_.push_back(token);
}

std::pair<bool, CJKeyword::Type>
CJavaScript::
isKeyword(const std::string &name) const
{
  CJKeyword::Type type = CJKeyword::nameToType(name);

  if (type != CJKeyword::Type::None)
    return std::pair<bool, CJKeyword::Type>(true, type);
  else
    return std::pair<bool, CJKeyword::Type>(false, CJKeyword::Type::None);
}

bool
CJavaScript::
isRealValueName(const std::string &name, bool allowUnary, CJNumber::RealType &realType) const
{
  auto p = nameRealTypeMap_.find(name);

  if (p == nameRealTypeMap_.end())
    return false;

  realType = (*p).second;

  if (! allowUnary && realType == CJNumber::RealType::NegInf)
    return false;

  return true;
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
  int binValue = 0; bool isBin = false;

  std::string realStr;

  // octal (0[0-7]*), hexadecimal (0[xX][0-F]*), or decimal (0.[0-9]*)
  if (parse.isChar('0')) {
    parse.skipChar();

    if      (parse.isOneOf("xX")) {
      isHex = true;

      parse.skipChar();

      while (! parse.eof() && parse.isXDigit()) {
        char c = parse.readChar();

        hexValue = 16*hexValue + CJUtil::hexCharValue(c);
      }
    }
    else if (parse.isOneOf("oO")) {
      isOct = true;

      parse.skipChar();

      while (! parse.eof() && parse.isODigit()) {
        char c = parse.readChar();

        octValue = 8*octValue + (c - '0');
      }
    }
    else if (parse.isOneOf("bB")) {
      isBin = true;

      parse.skipChar();

      while (! parse.eof() && parse.isOneOf("01")) {
        char c = parse.readChar();

        binValue = 2*binValue + (c - '0');
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

  if      (isOct) {
    number = createNumberValue(long(sign*octValue));
  }
  else if (isHex) {
    number = createNumberValue(long(sign*hexValue));
  }
  else if (isBin) {
    number = createNumberValue(long(sign*binValue));
  }
  else {
    const char *p;

    errno = 0;

    double real = strtod(realStr.c_str(), (char **) &p);

    int fpc = std::fpclassify(real);

    if      (fpc == FP_NAN)
      real = CJUtil::getNaN();
    else if (fpc == FP_INFINITE)
      real = (sign > 0 ? CJUtil::getPosInf() : CJUtil::getNegInf());
    else
      real = sign*real;

    number = createNumberValue(real);
  }

  number->setLineNum(parse.lineNum());

  tokens_.push_back(number);
}

bool
CJavaScript::
isOperatorName(const std::string &name, CJOperator::Type &type) const
{
  auto p = nameOperatorMap_.find(name);

  if (p == nameOperatorMap_.end())
    return false;

  type = (*p).second;

  return true;
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

  CJTokenP token = CJTokenP(new CJOperator(type, precedence, associativty, ary));

  token->setLineNum(parse.lineNum());

  tokens_.push_back(token);
}

void
CJavaScript::
readDoubleString(CStrParse &parse)
{
  std::string str;

  parse.skipChar();

  while (! parse.eof()) {
    if      (parse.isChar('\\')) {
      char c = parse.readChar();

      if (! parse.eof()) {
        c = parse.readChar();

        switch (c) {
          default : str += c; break; // ignore escape char

          case '\n': break;
          case 'b' : str += '\b'; break;
          case 'f' : str += '\f'; break;
          case 'n' : str += '\n'; break;
          case 'r' : str += '\r'; break;
          case 't' : str += '\t'; break;
          case 'v' : str += '\v'; break;
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
            // 4 hexadecimal digits
            // { <hexdigit> ... }
            int c1 = 0;
            int c2 = 0;
            int c3 = 0;
            int c4 = 0;

            if (! parse.eof() && parse.isXDigit())
              c1 = CJUtil::hexCharValue(parse.readChar());
            if (! parse.eof() && parse.isXDigit())
              c2 = CJUtil::hexCharValue(parse.readChar());
            if (! parse.eof() && parse.isXDigit())
              c3 = CJUtil::hexCharValue(parse.readChar());
            if (! parse.eof() && parse.isXDigit())
              c4 = CJUtil::hexCharValue(parse.readChar());

            long c = ((c1 & 0xF) << 12) |
                     ((c2 & 0xF) <<  8) |
                     ((c3 & 0xF) <<  4) |
                     ((c4 & 0xF) <<  0);

            CUtf8::append(str, c);

            break;
          }
          case 'x': {
            //2 hexadecimal digits
            int c1 = 0;
            int c2 = 0;

            if (! parse.eof() && parse.isXDigit())
              c1 = CJUtil::hexCharValue(parse.readChar());
            if (! parse.eof() && parse.isXDigit())
              c2 = CJUtil::hexCharValue(parse.readChar());

            long c = ((c1 & 0xF) << 4) | (c2 & 0xF);

            CUtf8::append(str, c);

            break;
          }
        }
      }
      else
        str += c;
    }
    else if (parse.isChar('"'))
      break;
    else {
      ulong l;

      if (! parse.readUtf8Char(&l))
        break;

      CUtf8::append(str, l);
    }
  }

  if (parse.isChar('"'))
    parse.skipChar();

  CJTokenP token = CJTokenP(new CJString(this, str, '\"'));

  token->setLineNum(parse.lineNum());

  tokens_.push_back(token);
}

void
CJavaScript::
readSingleString(CStrParse &parse)
{
  std::string str;

  parse.skipChar();

  while (! parse.eof() && ! parse.isChar('\'')) {
    if      (parse.isChar('\\')) {
      char c = parse.readChar();

      if (! parse.eof()) {
        c = parse.readChar();

        switch (c) {
          default : str += c; break; // ignore escape char

          case '\n': break;
          case 'b' : str += '\b'; break;
          case 'f' : str += '\f'; break;
          case 'n' : str += '\n'; break;
          case 'r' : str += '\r'; break;
          case 't' : str += '\t'; break;
          case 'v' : str += '\v'; break;
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
            // 4 hexadecimal digits
            // { <hexdigit> ... }
            int c1 = 0;
            int c2 = 0;
            int c3 = 0;
            int c4 = 0;

            if (! parse.eof() && parse.isXDigit())
              c1 = CJUtil::hexCharValue(parse.readChar());
            if (! parse.eof() && parse.isXDigit())
              c2 = CJUtil::hexCharValue(parse.readChar());
            if (! parse.eof() && parse.isXDigit())
              c3 = CJUtil::hexCharValue(parse.readChar());
            if (! parse.eof() && parse.isXDigit())
              c4 = CJUtil::hexCharValue(parse.readChar());

            long c = ((c1 & 0xF) << 12) |
                     ((c2 & 0xF) <<  8) |
                     ((c3 & 0xF) <<  4) |
                     ((c4 & 0xF) <<  0);

            CUtf8::append(str, c);

            break;
          }
          case 'x': {
            //2 hexadecimal digits
            int c1 = 0;
            int c2 = 0;

            if (! parse.eof() && parse.isXDigit())
              c1 = CJUtil::hexCharValue(parse.readChar());
            if (! parse.eof() && parse.isXDigit())
              c2 = CJUtil::hexCharValue(parse.readChar());

            long c = ((c1 & 0xF) << 4) | (c2 & 0xF);

            CUtf8::append(str, c);

            break;
          }
        }
      }
      else
        str += c;
    }
    else if (parse.isChar('\''))
      break;
    else {
      ulong l;

      if (! parse.readUtf8Char(&l))
        break;

      CUtf8::append(str, l);
    }
  }

  if (parse.isChar('\''))
    parse.skipChar();

  CJTokenP token = CJTokenP(new CJString(this, str, '\''));

  token->setLineNum(parse.lineNum());

  tokens_.push_back(token);
}

void
CJavaScript::
readRegExp(CStrParse &parse)
{
  std::string str;

  parse.skipChar();

  while (! parse.eof() && ! parse.isChar('/')) {
    if (parse.isChar('\\')) {
      char c = parse.readChar();

      if (! parse.eof()) {
        c = parse.readChar();

        if (c == '/')
          str += c;
        else {
          str += '\\';
          str += c;
        }
      }
      else
        str += '\\';
    }
    else
      str += parse.readChar();
  }

  if (parse.isChar('/'))
    parse.skipChar();

  CJTokenP token = CJTokenP(new CJRegExp(this, str));

  token->setLineNum(parse.lineNum());

  tokens_.push_back(token);
}

CJToken::Type
CJavaScript::
lastTokenType() const
{
  if (tokens_.empty())
    return CJToken::Type::None;

  return tokens_.back()->type();
}

void
CJavaScript::
throwException(CJExceptionType type)
{
  CJError *error = new CJError(this);

  error->setExceptionType(type);

  throwError(0, CJErrorBaseP(error));
}

void
CJavaScript::
throwTypeError(CJTokenP token, const std::string &msg)
{
  throwTypeError(token.get(), msg);
}

void
CJavaScript::
throwTypeError(CJToken *token, const std::string &msg)
{
  CJTypeError *error = new CJTypeError(this);

  error->setMessage(msg);

  throwError(token, CJErrorBaseP(error));
}

void
CJavaScript::
throwSyntaxError(CJTokenP token, const std::string &msg)
{
  throwSyntaxError(token.get(), msg);
}

void
CJavaScript::
throwSyntaxError(CJToken *token, const std::string &msg)
{
  CJSyntaxError *error = new CJSyntaxError(this);

  error->setMessage(msg);

  throwError(token, CJErrorBaseP(error));
}

void
CJavaScript::
throwReferenceError(CJTokenP token, const std::string &msg)
{
  throwReferenceError(token.get(), msg);
}

void
CJavaScript::
throwReferenceError(CJToken *token, const std::string &msg)
{
  CJReferenceError *error = new CJReferenceError(this);

  error->setMessage(msg);

  throwError(token, CJErrorBaseP(error));
}

void
CJavaScript::
throwError(CJToken *token, CJErrorBaseP error)
{
  if (getCurrentBlock()) {
    getCurrentBlock()->setError(error);

    if (isExceptDebug())
      errorMsg(token, error->message());
  }
  else {
    errorMsg(token, error->toString());
  }
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
  int lineNum = (token ? token->lineNum() : -1);

  if (fileName_ != "")
    std::cerr << fileName_ << "@" << lineNum << ": ";
  else
    std::cerr << lineNum << ": ";

  std::cerr << msg << std::endl;
}

void
CJavaScript::
errorMsg(const std::string &msg) const
{
  if (fileName_ != "")
    std::cerr << fileName_ << ": ";

  std::cerr << msg << std::endl;
}

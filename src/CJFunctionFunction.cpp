#include <CJFunctionFunction.h>
#include <CJFunction.h>
#include <CJavaScript.h>
#include <CStrParse.h>

CJFunctionFunction::
CJFunctionFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "Function", CJFunctionType::instance(js))
{
}

// new Function([args, ...], body);
CJValueP
CJFunctionFunction::
exec(CJavaScript *js, const Values &values)
{
  assert(values.size() >= 1);

  std::string name;
  // no name (anonymous function)

  // values[0] is CJUserObjectP

  // args are values[1]->values[values.size() - 2]
  CJFunction::Args args;

  if (values.size() > 2) {
    for (uint i = 1; i < values.size() - 1; ++i) {
      std::string arg = values[i]->toString();

      std::vector<std::string> args1;

      if (! expandArg(arg, args1)) {
        js->throwSyntaxError(this, "Invalid function arguments");
        return CJValueP();
      }

      for (const auto &arg1 : args1)
        args.push_back(arg1);
    }
  }

  // create function this global scope and return
  CJFunctionP userFn(new CJFunction(js, name, args));

  userFn->setScope(js, js->currentUserFunction());

  // body is values[values.size() - 1]
  CJExecBlockP block;

  if (values.size() > 1) {
    std::string body = values[values.size() - 1]->toString();

    js->pushUserFunction(userFn);

    //js->startScope(userFn->scope());

    block = js->interpFunctionBlock(body);

    //js->endScope();

    userFn->setBlock(block);

    js->popUserFunction();
  }

  return userFn;
}

bool
CJFunctionFunction::
expandArg(const std::string &arg, std::vector<std::string> &args) const
{
  CStrParse parse(arg);

  while (! parse.eof()) {
    parse.skipSpace();

    if (parse.isAlpha() || parse.isOneOf("_$")) {
      std::string arg1;

      while (! parse.eof() && (parse.isAlnum() || parse.isOneOf("_$"))) {
        arg1 += parse.readChar();
      }

      assert(arg1 != "");

      args.push_back(arg1);

      parse.skipSpace();
    }
    else
      return false;

    if (! parse.isChar(','))
      break;

    parse.skipChar();
  }

  parse.skipSpace();

  return parse.eof();
}

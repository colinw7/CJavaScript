#include <CJavaScript.h>
#include <CReadLine.h>
#include <cstring>
#include <iostream>

int
main(int argc, char **argv)
{
  std::string filename;

  bool pdebug = false;
  bool idebug = false;
  bool edebug = false;
  bool xdebug = false;
  bool parse  = false; // just parse
  bool fast   = false;
  bool prompt = false;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if      (strcmp(&argv[i][1], "pdebug") == 0)
        pdebug = true;
      else if (strcmp(&argv[i][1], "idebug") == 0)
        idebug = true;
      else if (strcmp(&argv[i][1], "edebug") == 0)
        edebug = true;
      else if (strcmp(&argv[i][1], "xdebug") == 0)
        xdebug = true;
      else if (strcmp(&argv[i][1], "debug") == 0) {
        pdebug = true;
        idebug = true;
        edebug = true;
        xdebug = true;
      }
      else if (strcmp(&argv[i][1], "parse") == 0)
        parse = true;
      else if (strcmp(&argv[i][1], "f") == 0)
        fast = true;
      else if (strcmp(&argv[i][1], "i") == 0)
        prompt = true;
      else
        std::cerr << "Invalid option: " << argv[i] << std::endl;
    }
    else
      filename = argv[i];
  }

  CJavaScript js;

  js.setParseDebug (pdebug);
  js.setInterpDebug(idebug);
  js.setExecDebug  (edebug);
  js.setExprDebug  (xdebug);

  if (! fast)
    js.loadStartpFile();

  if (! filename.empty()) {
    js.loadFile(filename);

    if (! parse) {
      CJValueP value = js.exec();

      if (value && value->type() != CJValue::Type::Undefined) {
        value->print(std::cout);
        std::cout << std::endl;
      }
    }
  }

  if (prompt) {
    CReadLine readline;

    readline.setAutoHistory(true);

    std::string line;
    int         depth = 0;

    while (true) {
      std::string prompt = ">";

      for (int i = 0; i < depth; ++i)
        prompt += "+";

      readline.setPrompt(prompt + " ");

      if (line != "")
        line += "\n";

      line += readline.readLine();

      if (line == "exit")
        break;

      depth = js.isCompleteLine(line);

      if (depth == 0) {
        if (! js.isEmptyLine(line)) {
          js.loadString(line);

          CJValueP value = js.exec();

          if (value) {
            value->print(std::cout);
            std::cout << std::endl;
          }
        }

        line = "";
      }
    }
  }

  return 0;
}

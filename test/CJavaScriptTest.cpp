#include <CJavaScript.h>
#include <CReadLine.h>
#include <cstring>
#include <iostream>

int
main(int argc, char **argv)
{
  std::string filename;

  bool pdebug = false;
  bool edebug = false;
  bool parse  = false; // just parse
  bool prompt = false;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if      (strcmp(&argv[i][1], "pdebug") == 0)
        pdebug = true;
      else if (strcmp(&argv[i][1], "edebug") == 0)
        edebug = true;
      else if (strcmp(&argv[i][1], "parse") == 0)
        parse = true;
      else if (strcmp(&argv[i][1], "i") == 0)
        prompt = true;
      else
        std::cerr << "Invalid option: " << argv[i] << std::endl;
    }
    else
      filename = argv[i];
  }

  CJavaScript js;

  js.setParseDebug(pdebug);
  js.setExecDebug (edebug);

  if (! filename.empty()) {
    js.loadFile(filename);

    if (! parse) {
      CJValueP value = js.exec();

      if (value) {
        value->print(std::cout);
        std::cout << std::endl;
      }
    }
  }

  if (prompt) {
    CReadLine readline;

    readline.setPrompt("> ");

    while (true) {
      std::string line = readline.readLine();

      if (line == "exit")
        break;

      js.loadString(line);

      CJValueP value = js.exec();

      if (value) {
        value->print(std::cout);
        std::cout << std::endl;
      }
    }
  }

  return 0;
}

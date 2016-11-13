#include <CQJavaScript.h>
#include <CQApp.h>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  CQJavaScript *qjs = new CQJavaScript;

  std::string filename;

  bool pdebug = false;
  bool idebug = false;
  bool edebug = false;
  bool xdebug = false;
  int  size   = -1;

  for (int i = 1; i < argc; ++i) {
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
      else if (strcmp(&argv[i][1], "size") == 0) {
        ++i;

        if (i < argc)
          size = std::max(1, atoi(argv[i]));
      }
      else
        std::cerr << "Invalid option: " << argv[i] << std::endl;
    }
    else
      filename = argv[i];
  }

  if (size > 0)
    qjs->setSize(size);

  qjs->init();

  if (size > 0)
    qjs->setFixedSize(QSize(size, size));

  qjs->show();

  CJavaScript *js = qjs->js();

  js->setParseDebug (pdebug);
  js->setInterpDebug(idebug);
  js->setExecDebug  (edebug);
  js->setExprDebug  (xdebug);

  if (filename != "") {
    js->loadFile(filename);

    js->exec();
  }

  qjs->callEventListeners("load", "onload");

  app.exec();

  return 0;
}

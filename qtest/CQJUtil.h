#ifndef CQJUtil_H
#define CQJUtil_H

#include <CStrParse.h>

namespace CQJUtil {
  inline QColor nameToColor(const std::string &name) {
    CStrParse parse(name);

    parse.skipSpace();

    if (parse.isString("rgb(")) {
      parse.skipChars(4);

      int r = 0, g = 0, b = 0;

      parse.skipSpace();
      parse.readInteger(&r);
      parse.skipSpace();

      if (parse.isChar(',')) {
        parse.skipChar();

        parse.skipSpace();
        parse.readInteger(&g);
        parse.skipSpace();

        if (parse.isChar(',')) {
          parse.skipChar();

          parse.skipSpace();
          parse.readInteger(&b);
          parse.skipSpace();

          if (parse.isChar(')'))
            parse.skipChar();
        }
      }

      QColor c(r, g, b);

      return c;
    }
    else {
      QColor c(name.c_str());

      return c;
    }
  }
}

#endif

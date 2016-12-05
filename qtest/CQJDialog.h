#ifndef CQJDialog_H
#define CQJDialog_H

#include <QFrame>

class CJavaScript;
class QTextEdit;
class CQHistoryLineEdit;
class QListWidget;
class QListWidgetItem;

class CQJDialog : public QFrame {
  Q_OBJECT

 public:
  CQJDialog(CJavaScript *js);

  QSize sizeHint() const;

 private slots:
  void historySlot(QListWidgetItem *item);

  void execCmd(const QString &cmd);

  void loadFile();

  void loadVariables();
  void loadFunctions();

 private:
  CJavaScript       *js_            { 0 };
  QTextEdit         *log_           { 0 };
  CQHistoryLineEdit *input_         { 0 };
  QListWidget       *history_       { 0 };
  QListWidget       *variablesList_ { 0 };
  QListWidget       *functionsList_ { 0 };
};

#endif

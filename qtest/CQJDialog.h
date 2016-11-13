#ifndef CQJDialog_H
#define CQJDialog_H

#include <QFrame>

class CQJavaScript;
class QTextEdit;
class CQHistoryLineEdit;
class QListWidget;
class QListWidgetItem;

class CQJDialog : public QFrame {
  Q_OBJECT

 public:
  CQJDialog(CQJavaScript *js);

  QSize sizeHint() const;

 private slots:
  void historySlot(QListWidgetItem *item);

  void execCmd(const QString &cmd);

  void loadFile();

  void loadVariables();
  void loadFunctions();

 private:
  CQJavaScript      *qjs_           { 0 };
  QTextEdit         *log_           { 0 };
  CQHistoryLineEdit *input_         { 0 };
  QListWidget       *history_       { 0 };
  QListWidget       *variablesList_ { 0 };
  QListWidget       *functionsList_ { 0 };
};

#endif

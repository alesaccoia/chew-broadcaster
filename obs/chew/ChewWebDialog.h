#ifndef CHEWWEBDIALOG_H
#define CHEWWEBDIALOG_H

#include <QDialog>
#include <QtWebEngineWidgets>
#include <QWebChannel>
#include "ChewHTMLProxy.h"

namespace Ui {
class ChewWebDialog;
}

class ChewWebDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ChewWebDialog(QWidget *parent = 0);
  ~ChewWebDialog();

  void navigateToUrl(QUrl url);

  ChewHTMLProxy* getChewHtmlProxy();

private:
  Ui::ChewWebDialog *ui;
  QWebEnginePage* mWebPage;
  QWebEngineView* mWebView;
  QWebChannel* mWebChannel;
  ChewHTMLProxy* mProxy;
};

#endif // CHEWWEBDIALOG_H

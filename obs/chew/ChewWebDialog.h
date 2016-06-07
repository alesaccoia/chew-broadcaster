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
  void deleteCookies();

  ChewHTMLProxy* getChewHtmlProxy();

private:

  // the webview doesn't listen to shortcuts "as is"
  void setupShortcuts();

  Ui::ChewWebDialog *ui;
  QWebEnginePage* mWebPage;
  QWebEngineView* mWebView;
  QWebChannel* mWebChannel;
  ChewHTMLProxy* mProxy;
};

#endif // CHEWWEBDIALOG_H

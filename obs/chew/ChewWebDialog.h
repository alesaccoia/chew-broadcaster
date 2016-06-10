#ifndef CHEWWEBDIALOG_H
#define CHEWWEBDIALOG_H

#include <QDialog>
#include <QtWebEngineWidgets>
#include <QWebChannel>
#include <QUrl>
#include <QTimer>

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
  void clearContent();

  QWebChannel* getWebChannel() { return mWebChannel; }

private slots:
  void loadFinished(bool wasOk);

  // called by a timer that refreshes the page
  void reloadLastPage();
  
private:

  // the webview doesn't listen to shortcuts "as is"
  void setupShortcuts();

  Ui::ChewWebDialog *ui;
  QWebEnginePage* mWebPage;
  QWebEngineView* mWebView;
  QWebEnginePage* mErrorPage;
  QWebChannel* mWebChannel;
  QUrl mLastUrl;
  QTimer mReloadTimer;
};

#endif // CHEWWEBDIALOG_H

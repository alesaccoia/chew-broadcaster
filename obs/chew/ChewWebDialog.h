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

  /// Navigates directly to the given URL
  void navigateToUrl(QUrl url);
  
  /**
   * Uses the redirector.html page with AJAX to redirect to the wanted URL.
   * We use mostly this one because it's impossible to get reliable 
   * connection error reporting with QWebEnginePage
   */
  void navigateToUrlWithRedirect(QUrl url);
  
  /// Deletes the cookies from the application directory
  void deleteCookies();
  
  /// Clears the content of the webview setting it to nothing
  void clearContent();

  /**
   * Returns the WebChannel associated with this Dialog. Used to associate
   * a proxy object
   */
  QWebChannel* getWebChannel() { return mWebChannel; }

signals:
  /*
   * The user code can return a bool preventing the window from closing
   */
  bool wantsToClose();

private:

  /// Sets up the shortcuts that aren't handled from the webview out of the box
  void setupShortcuts();

  /// Called when the X button is pressed
  void reject();

  Ui::ChewWebDialog *ui;
  QWebEnginePage* mWebPage;
  QWebEngineView* mWebView;
  QWebChannel* mWebChannel;
  
  QString redirectorPagePath;
};

#endif // CHEWWEBDIALOG_H

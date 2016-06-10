#include "ChewWebDialog.h"
#include "ui_ChewWebDialog.h"

ChewWebDialog::ChewWebDialog(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::ChewWebDialog)
{

  qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "23654");
  ui->setupUi(this);

  this->setFixedSize(640, 480);

  mWebPage = new QWebEnginePage();
  mRedirectorPage = new QWebEnginePage();
  mWebChannel = new QWebChannel();
  mWebView = new QWebEngineView(this);

  mWebPage->setView(mWebView);
  mWebPage->setWebChannel(mWebChannel);
  mWebView->setPage(mWebPage);
  
  // alex todo: stringify this and include it in a string
  redirectorPagePath = QCoreApplication::applicationDirPath() + "/../data/obs-studio/chew/redirector.html";

  //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  this->setModal(true);

  setupShortcuts();
}

ChewWebDialog::~ChewWebDialog() {
  delete ui;
}

void ChewWebDialog::setupShortcuts() {
// #if defined (Q_OS_MAC)
  QAction *shrtCut = mWebView->pageAction(QWebEnginePage::Cut);
  shrtCut->setShortcut(QKeySequence::Cut);
  addAction(shrtCut);
  QAction *shrtCopy = mWebView->pageAction(QWebEnginePage::Copy);;
  shrtCopy->setShortcut(QKeySequence::Copy);
  addAction(shrtCopy);
  QAction *shrtPaste = mWebView->pageAction(QWebEnginePage::Paste);
  shrtPaste->setShortcut(QKeySequence::Paste);
  addAction(shrtPaste);
  QAction *slctAll = mWebView->pageAction(QWebEnginePage::SelectAll);
  slctAll->setShortcut(QKeySequence::SelectAll);
  addAction(slctAll);
// #endif
}

void ChewWebDialog::navigateToUrl(QUrl url) {
  clearContent();
  
  mWebPage->load(url);
  mWebView->setPage(mWebPage);
  mWebView->resize(this->size());
  mWebView->update();
}

void ChewWebDialog::navigateToUrlWithRedirect(QUrl url) {
  clearContent();
  
  QString urlEncoded = url.encoded();
  QString fullUrl = redirectorPagePath + "?url=" + url.url();
  
  mWebPage->load(QUrl(fullUrl));
  mWebView->setPage(mWebPage);
  mWebView->resize(this->size());
  mWebView->update();
}

void ChewWebDialog::deleteCookies() {
 mWebPage->profile()->cookieStore()->deleteAllCookies();
}

void ChewWebDialog::clearContent() {
  mWebView->setHtml("");
  mWebView->update();
}


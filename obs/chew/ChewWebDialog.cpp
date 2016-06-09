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
  mWebChannel = new QWebChannel();
  mWebView = new QWebEngineView(this);

  mProxy = new ChewHTMLProxy;
  mWebPage->setView(mWebView);
  mWebPage->setWebChannel(mWebChannel);
  mWebView->setPage(mWebPage);

  //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  this->setModal(true);

  setupShortcuts();
}

ChewWebDialog::~ChewWebDialog()
{
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
  mWebView->setHtml("");
  mWebView->update();
  mWebPage->load(url);
  mWebView->setPage(mWebPage);
  mWebView->resize(this->size());
  update();
}

void ChewWebDialog::deleteCookies() {
 mWebPage->profile()->cookieStore()->deleteAllCookies();
}


ChewHTMLProxy* ChewWebDialog::getChewHtmlProxy() {
  return mProxy;
}

void ChewWebDialog::clearContent() {
  mWebView->setHtml("<html><body></body></html>");
}


#include "ChewWebDialog.h"
#include "ui_ChewWebDialog.h"

ChewWebDialog::ChewWebDialog(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::ChewWebDialog)
{
  qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "23655");
  ui->setupUi(this);

  this->setFixedSize(640, 480);
  this->setAttribute(Qt::WA_DeleteOnClose, false);

  mWebPage = new QWebEnginePage();
  mWebChannel = new QWebChannel();
  mWebView = new QWebEngineView(this);
  
  mWebPage->setView(mWebView);
  mWebPage->setWebChannel(mWebChannel);
  mWebPage->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
  
  mWebView->setPage(mWebPage);
  
  // alex todo: stringify this and include it in a string
  redirectorPagePath = "file://" + QCoreApplication::applicationDirPath() + "/../data/obs-studio/chew/redirectorSam.html";
  //redirectorPagePath = "file:///Volumes/Data/GoogleDrive/alsc.co/Clients/chew.tv/Chew Studio/graphics/html/redirector.html";
  //redirectorPagePath = "/Volumes/Data/WORK/alsc/Projects/chewtv/chew-studio/obs/chew/data/chew/redirector.html";
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
  
  QString fullUrl = redirectorPagePath + "?url=" + url.toString();
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

void ChewWebDialog::reject() {
  bool shouldClose = emit wantsToClose();
  if (shouldClose) QDialog::reject();
}

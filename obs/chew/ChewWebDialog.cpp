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
  mErrorPage = new QWebEnginePage();
  mWebChannel = new QWebChannel();
  mWebView = new QWebEngineView(this);

  mWebPage->setView(mWebView);
  mWebPage->setWebChannel(mWebChannel);
  mWebView->setPage(mWebPage);
  
  QObject::connect(mWebPage, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
  
  QString fullPath = QCoreApplication::applicationDirPath() + "/../data/obs-studio/chew/broadcaster-no-connection.html";
  
  qDebug() << fullPath;
  
  mErrorPage->load(QUrl::fromLocalFile(fullPath));

  //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  this->setModal(true);

  setupShortcuts();
}

ChewWebDialog::~ChewWebDialog() {
  delete ui;
}

void ChewWebDialog::loadFinished(bool wasOk) {
  if (wasOk) {
    qDebug() << "Loading OK " << mLastUrl;
    mWebView->setPage(mWebPage);
    mWebView->resize(this->size());
    mWebView->update();
  } else {
    qDebug() << "Error loading " << mLastUrl;
    mWebView->setPage(mErrorPage);
    mWebView->resize(this->size());
    mWebView->update();
    mReloadTimer.singleShot(2000, this, SLOT(reloadLastPage()));
  }
  update();
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

void ChewWebDialog::reloadLastPage() {
  mWebPage->load(mLastUrl);
  qDebug() << "Retrying loading " << mLastUrl;
}


void ChewWebDialog::navigateToUrl(QUrl url) {
  if (mReloadTimer.isActive()) {
    mReloadTimer.stop();
  }
  mLastUrl = url;

  mWebView->setHtml("");
  mWebView->update();
  mWebPage->load(url);
}

void ChewWebDialog::deleteCookies() {
 mWebPage->profile()->cookieStore()->deleteAllCookies();
}

void ChewWebDialog::clearContent() {
  mWebView->setHtml("<html><body></body></html>");
}


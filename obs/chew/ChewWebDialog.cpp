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

  mWebChannel->registerObject(QStringLiteral("app"), mProxy);
}

ChewWebDialog::~ChewWebDialog()
{
  delete ui;
}

void ChewWebDialog::navigateToUrl(QUrl url) {
  mWebPage->load(url);
  mWebView->setPage(mWebPage);
  mWebView->resize(this->size());
  update();
}

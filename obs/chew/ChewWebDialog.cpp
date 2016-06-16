#include "ChewWebDialog.h"
#include "ui_ChewWebDialog.h"
#include <QFocusEvent>

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
  
  redirectorPagePath = "file://" + QCoreApplication::applicationDirPath() + "/../data/obs-studio/chew/redirectorSam.html";
  
  //this->setModal(true);

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

///----------------------- FocusWidget -----------------------------///


FocusWidget::FocusWidget(QWidget *parent)
    : QWidget(parent)
    , childWidget(nullptr)
{
    setEnabled(true);
    setFocusPolicy(Qt::StrongFocus);

    // to catch, when childWidget (and its children) loses focus
    connect(qApp, &QApplication::focusChanged, this, &FocusWidget::onFocusChanged);
}

void FocusWidget::setChildWidget(QWidget *child)
{
    childWidget = child;
    childWidget->setParent(this);
    childWidget->setEnabled(false);
}

void FocusWidget::focusInEvent(QFocusEvent *event)
{
    // Make sure the rest of events are handled
    QWidget::focusInEvent(event);

    if (childWidget) {
        // transfer focus to childWidget
        childWidget->setEnabled(true);
        childWidget->setFocus(event->reason());
        setFocusPolicy(Qt::NoFocus);
    }
}

void FocusWidget::resizeEvent(QResizeEvent *event)
{
    if (childWidget)
        childWidget->resize(event->size());
}

QSize FocusWidget::sizeHint() const
{
    if (childWidget)
        return childWidget->sizeHint();
    return QWidget::sizeHint();
}

void FocusWidget::onFocusChanged(QWidget * /*old*/, QWidget *now)
{
    if (childWidget && childWidget->isEnabled() && now) {
        for (QWidget *p = now; p != nullptr; p = p->parentWidget()) {
            if (p == childWidget) return;
        }
        // childWidget and its children lost focus
        childWidget->setEnabled(false);
        setFocusPolicy(Qt::StrongFocus);
    }
}


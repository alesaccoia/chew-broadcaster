#include "ChewUtilities.h"

#include <QTimer>
#include <QEventLoop>

namespace chew {


InternetConnectionChecker::InternetConnectionChecker() {

}

void InternetConnectionChecker::run() {
  QNetworkRequest request;
  request.setUrl(QUrl("http://www.chew.tv"));

  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  QNetworkReply *reply = manager->get(request);
  connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
          this, SLOT(networkError(QNetworkReply::NetworkError)));

  connect(reply, SIGNAL(finished()),
          this, SLOT(requestFinished()));
}

void InternetConnectionChecker::networkError(QNetworkReply::NetworkError code) {
  emit statusChanged(false);
}

void InternetConnectionChecker::requestFinished() {
  emit statusChanged(true);
}


SynchronousRequestWithTimeout::SynchronousRequestWithTimeout(const QUrl& url, RequestMethod method, unsigned int milliseconds)
  : mUrl(url)
  , mMethod(method)
  , mTime(milliseconds) {
  

}

bool SynchronousRequestWithTimeout::run() {
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  QNetworkRequest request(mUrl);
  request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("text/html; charset=utf-8"));
  QByteArray data;
  QNetworkReply *reply = (mMethod == GET) ? manager->get(request) : manager->post(request, data);
  
  QTimer timer;
  timer.setSingleShot(true);
  QEventLoop loop;
  connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
  connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  timer.start(mTime);
  loop.exec();

  bool toReturn = false;
  if(timer.isActive()) {
    timer.stop();
    if(reply->error() > 0) {
      toReturn = false;
    } else {
      int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
      if (v >= 200 && v < 300) {  // Success
        toReturn = true;
      } else {
        toReturn = false;
      }
    }
  } else {
     // timeout
     disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
     reply->abort();
     toReturn = false;
     return false;
  }
  delete reply;
  delete manager;
  return toReturn;
}


} // namespace chew

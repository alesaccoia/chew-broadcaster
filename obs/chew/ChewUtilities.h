/**
 *  Chew.tv Utility classes
 *
 *  @project Chew Broadcaster
 *  @author Alessandro Saccoia <alessandro@alsc.co>
 *  @copyright chew.tv (2016)
 *  @license GPL 2.0
 *  @date 7/8/2016
 */

#ifndef chew_utilities_h_
#define chew_utilities_h_

#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace chew {

class InternetConnectionChecker : public QObject {
  Q_OBJECT
 public:
  InternetConnectionChecker();
  void run();
 signals:
  void statusChanged(bool ok);
 private slots:
  void networkError(QNetworkReply::NetworkError code);
  void requestFinished();
};

class SynchronousRequestWithTimeout : public QObject {
  Q_OBJECT
 public:
  SynchronousRequestWithTimeout(const QUrl& url, unsigned int milliseconds = 2000);
  bool run();
 private:
  QUrl mUrl;
  unsigned int mTime;
};

} // chew

#endif
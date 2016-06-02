#include "ChewHtmlProxy.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

const QString ChewHTMLProxy::operatingSystem_ = "Mac";
const QString ChewHTMLProxy::operatingSystemVersion_ = "10.9";
const QString ChewHTMLProxy::versionString_ = "1.0.0";
const int ChewHTMLProxy::versionMajor_ = 1;
const int ChewHTMLProxy::versionMinor_ = 0;
const int ChewHTMLProxy::versionBuild_ = 0;

ChewHTMLProxy::ChewHTMLProxy(QObject *parent)
   : QObject(parent)
{

}

void ChewHTMLProxy::execute(const QString &method, const QByteArray &params) {
  QJsonParseError err;
  QVariant jsonVariant= QJsonDocument::fromJson(params, &err).toVariant();
  if (err.error != QJsonParseError::NoError) {
    qDebug() << "There was an error with the Parsed JSON : " << err.errorString();
  }
#ifdef CHEW_DEBUG_WEBVIEW
  qDebug() << "Method:" << method
  QVariantMap paramsMap = params.toMap();
  QVariantMap::iterator i;
  for (i = paramsMap.begin(); i != paramsMap.end(); ++i) {
      qDebug() << i.key() << ": " << i.value().typeName();
  }
#endif
  emit executeJs(method, jsonVariant);
}

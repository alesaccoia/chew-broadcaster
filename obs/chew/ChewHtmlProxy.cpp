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
  printParamsRecursive(jsonVariant);
#endif
  emit executeJs(method, jsonVariant);
}

void ChewHTMLProxy::printParamsRecursive(QVariant &params) {
  if (params.canConvert<QVariantMap>()) {
    QVariantMap::iterator i;
    for (i = params.toMap().begin(); i != params.toMap().end(); ++i) {
      qDebug() << i.key();
      printParamsRecursive(i.value());
    }
  } else if (params.canConvert<QVariantList>()) {
    QVariantList::iterator vli;
    QVariantList vl = params.toList();
    qDebug() << "[";
    for (vli = vl.begin(); vli != vl.end(); ++vli) {
      printParamsRecursive(*vli);
    }
    qDebug() << "]";
  } else {
    qDebug() << params.typeName();
  }
}
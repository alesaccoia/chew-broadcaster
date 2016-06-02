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


void ChewHTMLProxy::executeString(const QString &method, const QString &params) {
  qDebug() << "executeString METHOD:" << method;
  qDebug() << "QString PARAMS:" << params;
}

void ChewHTMLProxy::executeVariant(const QString &method, const QVariant &params) {
  qDebug() << "executeVariant METHOD:" << method;
  qDebug() << "VARIANT OBJ:" << params;
}

void ChewHTMLProxy::executeJson(const QString &method, const QVariant &params) {
  qDebug() << "executeJson METHOD:" << method;
  QJsonValue json;
  json.fromVariant(params);
  qDebug() << "QJSON PARAMS:" << json;
}

void ChewHTMLProxy::execute(const QString &method, const QByteArray &params) {
  qDebug() << "executeJsonStringified METHOD:" << method;
  QJsonParseError err;
  QJsonDocument jsonParam = QJsonDocument::fromJson(params, &err);

  if (err.error == QJsonParseError::NoError) {
    QJsonObject jsonObject = jsonParam.object();
    QJsonValue testObj = jsonObject["username"];
    qDebug() << testObj.toString();
    for (auto jv: jsonObject) {
      qDebug() << jv.type();
    }
  } else {
    qDebug() << err.errorString();
  }
}


const QString& ChewHTMLProxy::executeJsonStringifiedWithReturn(const QString &method, const QByteArray &params) {
  QJsonObject object;
  object.insert("isTrue", true);
  QJsonDocument doc(object);
  return "doc.toJson()";
}

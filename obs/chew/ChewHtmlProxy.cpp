#include "ChewHtmlProxy.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>


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
  printParamsRecursive(params);
  emit executeJs(method, jsonVariant);
}

void ChewHTMLProxy::printParamsRecursive(const QVariant &params) {
  if (params.canConvert<QVariantMap>()) {
    QVariantMap paramsAsMap = params.toMap();
    QVariantMap::iterator i;
    for (i = paramsAsMap.begin(); i != paramsAsMap.end(); ++i) {
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
    qDebug() << "(" << params.typeName() << ")" << params;
  }
}

QVariant& ChewHTMLProxy::getProperties() {
  return mProperties;
}

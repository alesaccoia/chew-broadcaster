#ifndef CHEWHTMLPROXY_H
#define CHEWHTMLPROXY_H

#include <QObject>

class ChewHTMLProxy : public QObject
{
  Q_OBJECT

private:
  static const QString operatingSystem_;
  static const QString operatingSystemVersion_;
  static const QString versionString_;
  static const int versionMajor_;
  static const int versionMinor_;
  static const int versionBuild_;
  
public:
  explicit ChewHTMLProxy(QObject *parent = 0);
  
  // utility method for debug
  static void printParamsRecursive(const QVariant &params);

  Q_PROPERTY(QString operatingSystem MEMBER operatingSystem_ CONSTANT)
  Q_PROPERTY(QString operatingSystemVersion MEMBER operatingSystemVersion_ CONSTANT)
  Q_PROPERTY(QString versionString MEMBER versionString_ CONSTANT)
  Q_PROPERTY(int versionMajor MEMBER versionMajor_ CONSTANT)
  Q_PROPERTY(int versionMinor MEMBER versionMinor_ CONSTANT)
  Q_PROPERTY(int versionBuild MEMBER versionBuild_ CONSTANT)

signals:
  void executeJs(const QString &method, const QVariant &params);

public slots:
  /** This slot is invoked from JS, executes the C++ implementation.
   *  @param method A method name
   *  @param params
   *
   *  @note JS call goes like:
   *
   *  app.execute(
   *    'doSomething',
   *    {arg1: 1, arg2: 2},
   *    function(returnValue) {
   *      console.log(returnValue);
   *    }
   *  );
   *
   */
  void execute(const QString &method, const QByteArray &params);
};

/*
 * Macros for dealing less verbosely with the qvariantmap that represent the JSON values
 */

#define chew_check_and_convert_variant_map(varIn, vrmOut) \
  if (!varIn.canConvert<QVariantMap>()) { \
    qDebug() << "JSON for open URL malformed? Impossible to convert to QVariantMap"; \
    return; \
  } \
  vrmOut = varIn.toMap();

#define chew_check_and_return_variant(vrmIn, varOut, varFld) \
  if (!vrmIn.contains(varFld)) { \
    qDebug() << "Impossible to find " varFld " in the open URL parameters"; \
    return; \
  } \
  varOut = vrmIn.value(varFld);




#endif // CHEWHTMLPROXY_H

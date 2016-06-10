#ifndef CHEWHTMLPROXY_H
#define CHEWHTMLPROXY_H

#include <QObject>
#include <QVariant>

class ChewHTMLProxy : public QObject
{
  Q_OBJECT
  
public:
  explicit ChewHTMLProxy(QObject *parent = 0);

  // exposed on the Javascript side
  Q_PROPERTY(QVariant properties MEMBER mProperties CONSTANT)
  
  // returns the properties object in order to populate it
  QVariant& getProperties();
  
  // utility method for debug
  static void printParamsRecursive(const QVariant &params);
  
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

private:
  QVariant mProperties;

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

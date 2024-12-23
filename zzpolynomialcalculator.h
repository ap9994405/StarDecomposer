#ifndef ZZPOLYNOMIALCALCULATOR_H
#define ZZPOLYNOMIALCALCULATOR_H

#include <QThread>
#include "stringpolynomial.h"
#include <QProcess>

class GraphModel;

class ZZPolynomialCalculator : public QThread
{
    Q_OBJECT
public:
    ZZPolynomialCalculator(GraphModel* model, QObject *parent = 0);
    Polynomial<QString> getPoly();
    Polynomial<QString> getPoly2();
    QString getTotal() const;
    QString getTotal2() const;

    bool hasError();
    const QString& getErrorMsg();
    void stop();
    void serialRun();
protected:
    void run();
    void run2();
signals:
    void resultReady(const QString &s);
public slots:
private slots:
//    void jobFinishedError(QProcess::ProcessError error );
//    void jobFinished( int exitCode, QProcess::ExitStatus exitStatus);
private:
    GraphModel* m_model;
    Polynomial<QString> m_poly;
    Polynomial<QString> m_poly2;
    QString m_total;
    QString m_total2;
    bool m_hasError;
    QString m_errorMsg;
    QProcess *m_process;
//    QList<QList<std::array<int, 6> > > m_clar_strs;
};

#endif // ZZPOLYNOMIALCALCULATOR_H

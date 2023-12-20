#include "zzpolynomialcalculator.h"

#include "graphmodel.h"
#include <QtWidgets>
#include <QtXml>
#include <QDebug>
#include <QThread>
#include <iostream>

#include <fstream>

using namespace std;



ZZPolynomialCalculator::ZZPolynomialCalculator(GraphModel* model, QObject *parent) :
    QThread(parent), m_model(model), m_hasError(false)
{
    this->setTerminationEnabled(true);
//    m_process = NULL ;
//    connect(m_process,SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(jobFinished(int,QProcess::ExitStatus)));
//    connect(m_process,SIGNAL(error(QProcess::ProcessError)), this, SLOT(jobFinishedError(QProcess::ProcessError)));
}

void ZZPolynomialCalculator::run()
{
    m_process = new QProcess();

    QString zzcalcname = "StarCalculator";


    m_poly.clear();
    QFileInfo cmdInfo = QFileInfo(QFileInfo(QCoreApplication::arguments().at(0)).absolutePath(),zzcalcname);

    if (!cmdInfo.exists() || !cmdInfo.isExecutable())
    {
        m_hasError = true;
        m_errorMsg = zzcalcname + " not found in the same path as ZZDecomposer";
        return;
    }
    QString cmd = cmdInfo.absoluteFilePath() ;

    // int nthreads = QThread::idealThreadCount() - 1;
    // if (nthreads < 1 )
    //     nthreads = 1;



    QStringList arguments;
    // arguments << "-x" << QString("%1").arg(nthreads);
    arguments << "-x" << "-B";
    QTemporaryFile file;    //temporaryfile
    if (file.open())
    {
        arguments << file.fileName();
        {
            QTextStream inOut(&file);
            inOut << m_model->getZZInput(true, false);  // GraphModel::getZZInput(bool connections, bool hydrogens)
            // getZZInput return xyzfile, if connections is true return xyzfile and neighborlist
            qDebug() << m_model->getZZInput(true, false);
            std::string ZZInput = m_model->getZZInput(true, false).toStdString();
            std::ofstream InputData;
            int i=0;
            while (true)
            {   
                i += 1;
                std::string FileName = std::string("./Output/Output") + std::to_string(i) + std::string(".txt");
                std::ifstream checkfile(FileName);
                if (checkfile.good())   //check if file can open
                {   
                    checkfile.close();
                    continue;
                }
                else
                {
                    InputData.open(FileName);
                    if (InputData.is_open()) {
                        InputData << ZZInput << std::endl;  
                        InputData.close();
                        // std::cout << "File written successfully." << std::endl;
                    } 
                    else {
                        std::cerr << "Failed to open the file." << std::endl;
                    }
                    break;
                }
            }
        }

        m_process->start(cmd, arguments);
        m_process->waitForFinished(-1);

        if (m_process->exitCode()!=0)
        {
            m_hasError = true;
            m_errorMsg = zzcalcname + " crashed.";
            return;
        }

        QByteArray result = m_process->readAll();
        QString t(result);
        QDomDocument resultXML;
        bool failed = !resultXML.setContent(t);
        if (failed)
        {
            m_hasError = true;
            m_errorMsg = "The output from " + zzcalcname + " has wrong format1.";
            return;
        }
        else
        {
            int i=0;
            while (true)
            {   
                i += 1;
                std::string FileName = std::string("./Output/Output") + std::to_string(i) + std::string(".txt");
                std::ifstream checkfile(FileName);  // read
                std::ofstream OutputData;  // write
                if (checkfile.good())
                {
                    checkfile.close();
                    continue;
                }
                else
                {
                    i -= 1;
                    FileName = std::string("./Output/Output") + std::to_string(i) + std::string(".txt");
                    OutputData.open(FileName, std::ios::app);
                    if (OutputData.is_open()) 
                    {
                        OutputData << "-------------------------------------------" << std::endl;
                        OutputData << t.toStdString();
                        OutputData.close();
                    } 
                    break;
                }
            }
        }

        QDomElement zzpoly = resultXML.documentElement();
        if (zzpoly.isNull() || zzpoly.tagName() != "zzpolynomial")
        {
            m_hasError = true;
            m_errorMsg = "The output from " + zzcalcname + " has wrong format2.";
            qDebug() << t;
            return;
        }

        QDomNodeList terms = zzpoly.elementsByTagName("term");
        QDomNodeList total = zzpoly.elementsByTagName("total");

        if (total.count() == 1)
        {
            QDomElement total_elem = total.at(0).toElement();
            QString total_str = total_elem.text();
            m_total = total_str;
        }
        for (int i = 0; i < terms.count(); ++i)
        {
            QDomNode termNode = terms.item(i);
            QDomElement termElem = termNode.toElement();
            QDomNodeList orders = termElem.elementsByTagName("order");
            QDomNodeList coeffs = termElem.elementsByTagName("coefficient");
            if (orders.count() != 1 || coeffs.count() != 1)
            {
                m_hasError = true;
                m_errorMsg = "The output from " + zzcalcname + " has wrong format3.";
                qDebug() << t;
                return;
            }
            QDomElement orderElem = orders.at(0).toElement();
            QDomElement coeffElem = coeffs.at(0).toElement();

            int order = orderElem.text().toInt();
            QString coeff = coeffElem.text();
            
            m_poly.addTerm(coeff,order);
        }
    }
    delete m_process;
}

Polynomial<QString> ZZPolynomialCalculator::getPoly()
{
    return m_poly;
}

QString ZZPolynomialCalculator::getTotal() const
{
    return m_total;
}

bool ZZPolynomialCalculator::hasError()
{
    return m_hasError;
}

const QString & ZZPolynomialCalculator::getErrorMsg()
{
    return m_errorMsg;
}

void ZZPolynomialCalculator::stop()
{
    m_process->kill();
}

void ZZPolynomialCalculator::serialRun()
{
    run();
}


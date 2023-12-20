#ifndef STRINGPOLYNOMIAL_H
#define STRINGPOLYNOMIAL_H

#include <QString>
#include <QDebug>
#include "polynomial.h"
#include <tuple>


template<>
class Polynomial<QString>
{
    typedef QPair<QString,unsigned> Term;
    
public:
    void clear()
    {
        m_terms.clear();
    }

    Polynomial<QString>(){}

    Polynomial<QString>(const Polynomial<QString>& rhs)
    {
        m_terms = rhs.m_terms;
    }
    void addTerm(QString coeff,unsigned order)
    {
        if (coeff == 0) return;
        bool found_term = false;

        QList<Term>::iterator p;
        for(p=m_terms.begin();p!=m_terms.end();++p)
        {
            Term& term = *p;
            if (term.second == order)
            {
                term.first += coeff;
                found_term = true;
                break;
            }
            else if (term.second < order)
            {
                m_terms.insert(p,qMakePair(coeff,order));
                found_term = true;
                break;
            }
        }
        if (!found_term)
        {
            m_terms.push_back(qMakePair(coeff,order));
        }
    }

    bool isIdentity()
    {
        if (m_terms.size()>1)
            return false;
        QString a = getCoeff(0);
        if (a == "")
            return true;
        else
            return false;
    }

    Polynomial<QString>& operator=(const Polynomial<QString>& rhs)
    {
        m_terms = rhs.m_terms;
        return *this;
    }
    
    int degree()
    {
        int d = 0;
        QListIterator<Term> it(m_terms);
        while(it.hasNext())
        {
            int o = it.next().second;
            if (o > d)
            {
                d = o;
            }
        }
        return d;
    }

    QString getCoeff(unsigned order) const
    {
        QString def = "0";
        QListIterator<Term> it(m_terms);
        while(it.hasNext())
        {
            const Term& term = it.next();
            if (term.second == order)
            {
                return term.first;
            }
        }
        return def;
    }
    QString toString()
    {
        QStringList list;

        if ( m_terms.isEmpty() )
        {
            list.append("0");
        }
        else
        {
            QListIterator<Term> it(m_terms);
            while(it.hasNext())
            {
                const Term& term = it.next();
                if (term.second >1)
                {
                    if (term.first == "1")
                    {
                        list.append(QString("x^%1").arg(term.second));
                    }
                    else
                    {
                        list.append(QString("%1*x^%2").arg(term.first).arg(term.second));
                    }
                }
                else if ( term.second > 0)
                {
                    if (term.first == "1")
                    {
                        list.append(QString("x"));
                    }
                    else
                    {
                        list.append(QString("%1*x").arg(term.first));
                    }
                }
                else
                {
                    list.append(QString("%1").arg(term.first));
                }
            }
        }
        return list.join(" + ");
    }

    bool isEmpty()
    {
        return m_terms.isEmpty();
    }
    int numOfTerms()
    {
        return m_terms.size();
    }
    QString toHTML() const
    {
        QStringList list;
        if ( m_terms.isEmpty() )
        {
            // qDebug() << "m_terms is Empty";
            list.append("0");
        }
        else
        {
            QListIterator<Term> it(m_terms);
            while(it.hasNext())
            {
                const Term& term = it.next();
                if (term.second >1)
                {
                    if (term.first == "1")
                    {
                        list.append(QString("<i>x</i><sup>%1</sup>").arg(term.second));
                    }
                    else
                    {
                        list.append(QString("%1<i>x</i><sup>%2</sup>").arg(term.first).arg(term.second));
                    }
                }
                else if ( term.second > 0)
                {
                    if (term.first == "1")
                    {
                        list.append(QString("<i>x</i>"));
                    }
                    else
                    {
                        list.append(QString("%1<i>x</i>").arg(term.first));
                    }
                }
                else
                {
                    list.append(QString("%1").arg(term.first));
                }
            }
        }
        return list.join(" + ");
    }

    QString toHTML2() const
    {
        QStringList list;
        if ( m_terms.isEmpty() )
        {
            // qDebug() << "m_terms is Empty";
            list.append("0");
        }
        else
        {
            QListIterator<Term> it(m_terms);
            while(it.hasNext())
            {
                const Term& term = it.next();
                if (term.second >1)
                {
                    if (term.first == "1")
                    {
                        list.append(QString("<i>x</i>^%1").arg(term.second));
                    }
                    else
                    {
                        list.append(QString("%1*<i>x</i>^%2").arg(term.first).arg(term.second));
                    }
                }
                else if ( term.second > 0)
                {
                    if (term.first == "1")
                    {
                        list.append(QString("<i>x</i>"));
                    }
                    else
                    {
                        list.append(QString("%1*<i>x</i>").arg(term.first));
                    }
                }
                else
                {
                    list.append(QString("%1").arg(term.first));
                }
            }
        }
        return list.join(" + ");
    }

private:
    QList<Term> m_terms;
};
#endif // STRINGPOLYNOMIAL_H

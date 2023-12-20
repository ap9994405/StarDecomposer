#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <QList>
#include <QPair>
// #include <QDebug>
#include <QStringList>
#include <tuple>

template<typename X>
class Polynomial
{
    typedef QPair<X,unsigned> Term;
    typedef std::tuple <X, unsigned, unsigned> GTerm;
public:
    void clear()
    {
        m_terms.clear();
        m_gterms.clear();
    }

    Polynomial<X>(){}

    Polynomial<X>(const Polynomial<X>& rhs)
    {
        m_terms = rhs.m_terms;
        m_gterms = rhs.m_gterms;
    }

    Polynomial<X> operator+(const Polynomial<X>& rhs)
    {
        Polynomial<X> new_poly(*this);
        new_poly+=rhs;
        return new_poly;
    }
    Polynomial<X> operator*(const Polynomial<X>& rhs)
    {
        
        Polynomial<X> new_poly(*this);
        new_poly*=rhs;
        return new_poly;
    }

    Polynomial<X>& operator+=(const Polynomial<X>& rhs)
    {
        QListIterator<Term> it(rhs.m_terms);
        while(it.hasNext())
        {  
            const Term& term = it.next();
            addTerm(term.first,term.second);
        }
        return *this;
    }
    Polynomial<X>& operator*=(const Polynomial<X>& rhs)
    {
        QList<Term> old_terms;
        old_terms.swap(m_terms);

        QListIterator<Term> it(rhs.m_terms);

        while(it.hasNext())
        {
            const Term& term_2 = it.next();
            QListIterator<Term> it2(old_terms);
            while(it2.hasNext())
            {
                const Term& term_1 = it2.next();
                addTerm(term_2.first*term_1.first,term_2.second+term_1.second);
            }
        }
        return *this;
    }
    void multiplyTerm(X coeff,unsigned order)
    {
        if (coeff == 0) m_terms.clear();
        QMutableListIterator<Term> it(m_terms);
        while(it.hasNext())
        {
            Term& term = it.next();
            term.second += order;
            term.first *= coeff;
        }

    }
    void addTerm(X coeff,unsigned order)
    {
        if (coeff == 0) return;
        bool found_term = false;
        // qDebug() << "polynomial.h addTerm" ;
        typename QList<Term>::iterator p;
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

    void addTerm2(X coeff,unsigned order_x,unsigned order_y)
    {
        if (coeff == 0) return;
        bool found_term = false;
        typename QList<GTerm>::iterator p;
        for(p=m_gterms.begin();p!=m_gterms.end();++p)
        {   
            GTerm& gterm = *p;
            if (std::get<1>(gterm) == order_x && std::get<2>(gterm) == order_y )//|| std::get<2>(gterm) == order_y
            {
                std::get<0>(gterm) += coeff;
                found_term = true;
                break;
            }
            else if (std::get<1>(gterm) < order_x || std::get<2>(gterm) < order_y)
            {
                m_gterms.insert(p,std::make_tuple(coeff, order_x, order_y));
                found_term = true;
                break;
            }
        }
        if (!found_term)
        {   
            // qDebug() << "test3" <<coeff << order_x << order_y;
            // qDebug() << "c";
            m_gterms.push_back(std::make_tuple(coeff, order_x, order_y));
        }
    }

    bool isIdentity() const
    {
        if (m_terms.size()>1)
            return false;
        X a = getCoeff(0);
        if (a == X(1))
            return true;
        else
            return false;
    }

    Polynomial<X>& operator=(const Polynomial<X>& rhs)
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

    X getCoeff(unsigned order) const
    {
        X def(0);
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

    QString toHTML() const
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
                    if (term.first == 1)
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
                    if (term.first == 1)
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
        if ( m_gterms.isEmpty() )
        {   
            list.append("0");
        }
        else
        {
            QListIterator<GTerm> it(m_gterms);
            while(it.hasNext())
            {
                const GTerm& gterm = it.next();
                if ( std::get<1>(gterm) >1 && std::get<2>(gterm) >1)
                {
                    if (std::get<0>(gterm) == "1")
                    {
                        list.append(QString("<i>x</i><sup>%1</sup><i>y</i><sup>%2</sup>").arg(std::get<1>(gterm)).arg(std::get<2>(gterm)));
                    }
                    else
                    {
                        list.append(QString("%1<i>x</i><sup>%2</sup><i>y</i><sup>%3</sup>").arg(std::get<0>(gterm)).arg(std::get<1>(gterm)).arg(std::get<2>(gterm)));
                    }
                }
                else if ( std::get<1>(gterm) == 1 &&  std::get<2>(gterm) > 1)
                {
                    if (std::get<0>(gterm) == "1")
                    {
                        list.append(QString("<i>x</i><i>y</i><sup>%1</sup>").arg(std::get<2>(gterm)));
                    }
                    else
                    {
                        list.append(QString("%1<i>x</i><i>y</i><sup>%2</sup>").arg(std::get<0>(gterm)).arg(std::get<2>(gterm)));
                    }
                }
                else if ( std::get<1>(gterm) > 1 && std::get<2>(gterm) == 1)
                {
                    if (std::get<0>(gterm) == "1")
                    {
                        list.append(QString("<i>x</i><sup>%1</sup><i>y</i>").arg(std::get<1>(gterm)));
                    }
                    else
                    {
                        list.append(QString("%1<i>x</i><sup>%2</sup><i>y</i>").arg(std::get<0>(gterm)).arg(std::get<1>(gterm)));
                    }
                }
                else if ( std::get<1>(gterm) == 1 && std::get<2>(gterm) == 1)
                {
                    if (std::get<0>(gterm) == "1")
                    {
                        list.append(QString("<i>x</i><i>y</i>"));
                    }
                    else
                    {
                        list.append(QString("%1<i>x</i><i>y</i>").arg(std::get<0>(gterm)));
                    }
                }
                else if ( std::get<1>(gterm) == 0 && std::get<2>(gterm) > 1)
                {
                    if (std::get<0>(gterm) == "1")
                    {
                        list.append(QString("<i>y</i><sup>%1</sup>").arg(std::get<2>(gterm)));
                    }
                    else
                    {
                        list.append(QString("%1<i>y</i><sup>%2</sup>").arg(std::get<0>(gterm)).arg(std::get<2>(gterm)));
                    }
                }
                else if ( std::get<1>(gterm) > 1 && std::get<2>(gterm) == 0)
                {
                    if (std::get<0>(gterm) == "1")
                    {
                        list.append(QString("<i>x</i><sup>%1</sup>").arg(std::get<1>(gterm)));
                    }
                    else
                    {
                        list.append(QString("%1<i>x</i><sup>%2</sup>").arg(std::get<0>(gterm)).arg(std::get<1>(gterm)));
                    }
                }
                else if ( std::get<1>(gterm) == 1 && std::get<2>(gterm) == 0)
                {
                    if (std::get<0>(gterm) == "1")
                    {
                        list.append(QString("<i>x</i>"));
                    }
                    else
                    {
                        list.append(QString("%1<i>x</i>").arg(std::get<0>(gterm)));
                    }
                }
                else if ( std::get<1>(gterm) == 0 && std::get<2>(gterm) == 1)
                {
                    if (std::get<0>(gterm) == "1")
                    {
                        list.append(QString("<i>y</i>"));
                    }
                    else
                    {
                        list.append(QString("%1<i>y</i>").arg(std::get<0>(gterm)));
                    }                
                }
                else
                {
                    list.append(QString("%1").arg(std::get<0>(gterm)));
                }
            }
        }
        return list.join(" + ");
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
                    if (term.first == 1)
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
                    if (term.first == 1)
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

protected:
private:
    QList<Term> m_terms;
    QList<GTerm> m_gterms;
};




#endif // POLYNOMIAL_H

#ifndef ZZPOLYNOMIALRESULTDELEGATE_H
#define ZZPOLYNOMIALRESULTDELEGATE_H

#include <QAbstractItemDelegate>
#include <QStaticText>

class ZZPolynomialResultDelegate : public QAbstractItemDelegate
{
public:
    ZZPolynomialResultDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    ~ZZPolynomialResultDelegate();
private:
    QStaticText *text;
};

#endif // ZZPOLYNOMIALRESULTDELEGATE_H

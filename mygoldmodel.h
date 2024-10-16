#ifndef MYGOLDMODEL_H
#define MYGOLDMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include "golditem.h"
#include <QList>

class MyGoldModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MyGoldModel(QList<GoldItem> *list, QObject *parent = nullptr) : QAbstractTableModel(parent)
    {
        m_list = list;
    }

    bool loadCSV(QString *fileName) {

    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED (parent);
        return m_list->size ();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if(role == Qt::DisplayRole)
        {
            GoldItem item = m_list->at(index.row());
            if(index.column () == 0) {
                return item.DateTime ();
            }
            if(index.column () == 1) {
                return item.PricePurchase ();
            }
            if(index.column () == 2) {
                return item.PriceSell ();
            }
        }
        return QVariant();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED (parent);
        return 3;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        Q_UNUSED (orientation);
        if(role == Qt::DisplayRole) {
            if(section == 0) {
                return "Дата-время";
            }
            if(section == 1) {
                return  "Цена продажи";
            }
            if(section == 2) {
                return "Цена покупки";
            }
        }
        return QVariant();
    }



private:
    QList<GoldItem> *m_list;

signals:

};

#endif // MYGOLDMODEL_H


/*
 * DateTime,PricePurchase,PriceSell
 */

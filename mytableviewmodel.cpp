#include "mytableviewmodel.h"
#include "dbmanager.h"
#include <QSqlQuery>
#include <QDebug>
#include <QColor>
#include <twroleselectdelegate.h>

QVariant MyTableViewModel::data(const QModelIndex &index, int role) const
{
    //после редактирования надо обновить модель
    connect (this, &MyTableViewModel::dataChanged, this, &MyTableViewModel::updateModel);
    QVariant value = QSqlQueryModel::data(index, role);

    switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            if(index.column () == 1) {
                return value;
                //return value.toString ().prepend (tr("+7"));
            }
            else return value;
        case Qt::TextAlignmentRole:
            return int(Qt::AlignHCenter | Qt::AlignVCenter);
        case Qt::BackgroundColorRole:
            if(index.column () == 2) {
                if(index.data ().toString ().contains ("+7926")) {
                    return QColor(0, 255, 0);
                }
                if(index.data ().toString ().contains ("+7916")) {
                    return QColor(255, 0, 0);
                }
                if(index.data ().toString ().contains ("+7903")) {
                    return QColor(Qt::cyan);
                }
            }
            else if(index.column () == 0) {
                return QColor(200, 200, 200);
            }
            else return value;
        case Qt::ToolTipRole:
            if(index.column () == 2) {
                if(index.data ().toString ().contains ("+7926")) {
                    return "Мегафон";
                }
                if(index.data ().toString ().contains ("+7916")) {
                    return "МТС";
                }
                if(index.data ().toString ().contains ("+7903")) {
                    return "Билайн";
                }
            }
        }
    return value;
}

Qt::ItemFlags MyTableViewModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if(index.column () >= 1 && index.column () <= 4) {
        flags |= Qt::ItemIsEditable;
    }
    if(index.column () == 0) {
        flags |= Qt::ItemIsSelectable;
    }
    return flags;
}

MyTableViewModel::MyTableViewModel(QObject *parent, QSqlDatabase db) : QSqlTableModel (parent, db)
{
    this->updateModel ();
}

void MyTableViewModel::updateModel()
{
    QString sql = "SELECT " DB_ID_COLUMN ", "
                                         "" DB_NAME_COLUMN ", "
                                         "" DB_PHONE_COLUMN ", "
                                         "" DB_EMAIL_COLUMN ", "
                                         "" DB_ROLE_COLUMN "  "
                                         " FROM " DB_TABLE_NAME;
    QSqlQuery query = QSqlQuery(sql);
    qDebug() << sql << " " << query.result ();
    this->setQuery(query);
}

int MyTableViewModel::getId(int row)
{
    return this->data(this->index(row, 0), IdRole).toInt();
}

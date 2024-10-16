#include "rolemodel.h"
#include <QDebug>

RoleModel::RoleModel(QObject *parent) : QAbstractListModel (parent)
{
    m_content << qMakePair(DataPair::first_type("USER"), DataPair::second_type("user"))
    << qMakePair(DataPair::first_type("ADMIN"), DataPair::second_type("admin"));
}

QVariant RoleModel::data(const QModelIndex &index, int role) const
{
    const DataPair& data = m_content.at( index.row() );
    QVariant value;

    switch ( role )
    {
        case Qt::DisplayRole:
            value = data.first;
            break;

        case Qt::UserRole:
            value = data.second;
            break;

        default:
            break;
        }
        qDebug() << value;
        return value;
}

int RoleModel::rowCount(const QModelIndex &parent) const
{
    qDebug() << m_content.count ();
    return m_content.count ();
}

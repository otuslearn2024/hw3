#ifndef ROLEMODEL_H
#define ROLEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QString>

class RoleModel : public QAbstractListModel
{
    Q_OBJECT

private:
    typedef QPair<QString, QString> DataPair;
    QList< DataPair > m_content;

public:
    explicit RoleModel( QObject *parent = nullptr );
    virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
};

#endif // ROLEMODEL_H

#ifndef MYTABLEVIEWMODEL_H
#define MYTABLEVIEWMODEL_H

#include <QObject>
#include <QSqlTableModel>

class MyTableViewModel : public QSqlTableModel
{
    Q_OBJECT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        PhoneRole,
        EmailRole,
        RoleRole
    };

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    MyTableViewModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());



signals:

public slots:
    void updateModel();
    int getId(int row);
};

#endif // MYTABLEVIEWMODEL_H

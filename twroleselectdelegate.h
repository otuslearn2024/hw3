#ifndef TWROLESELECTDELEGATE_H
#define TWROLESELECTDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QComboBox>

class TWRoleSelectDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TWRoleSelectDelegate(QObject* parent = nullptr);
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    //QComboBox* m_comboBox;

};



#endif // TWROLESELECTDELEGATE_H

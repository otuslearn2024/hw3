#include "twroleselectdelegate.h"
#include "rolemodel.h"

TWRoleSelectDelegate::TWRoleSelectDelegate(QObject* parent) : QItemDelegate (parent)
{

}

QWidget *TWRoleSelectDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox* editor = new QComboBox(parent);
    editor->installEventFilter(const_cast<TWRoleSelectDelegate*>(this));
    return editor;
}

void TWRoleSelectDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    RoleModel *roles = new RoleModel();
    static_cast<QComboBox*>(editor)->setModel (roles);
}

void TWRoleSelectDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QString data = static_cast<QComboBox*>(editor)->currentText ();
    model->setData (index, data);
}

void TWRoleSelectDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

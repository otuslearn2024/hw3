#include "twadddialog.h"
#include <QBoxLayout>
#include <QObject>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QLabel>
#include "rolemodel.h"

TWAddDialog::TWAddDialog(QWidget *parent) : QDialog (parent)
{
    setModal (true);
    QBoxLayout* layout = new QVBoxLayout();

    m_name = new QLineEdit();
    m_email = new QLineEdit();
    m_phone = new QLineEdit();
    m_role = new QComboBox();

    QRegularExpression rx("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",
                          QRegularExpression::CaseInsensitiveOption);
    m_email->setValidator(new QRegularExpressionValidator(rx, this));
    connect(m_email, &QLineEdit::textChanged, this, &TWAddDialog::adjustTextColor);

    m_phone->setInputMask ("+79999999999");

    QPushButton* ok_btn = new QPushButton("Добавить");
    connect( ok_btn, &QPushButton::clicked, [&]() {
        if(!m_email->hasAcceptableInput()) {
            QMessageBox::warning(this, tr("Email error"),
                                 tr("Email is incorrect."), QMessageBox::Ok);
            return;
        }
        else if(m_name->text().isEmpty ()) {
            QMessageBox::warning(this, tr("Name error"),
                                 tr("Name is empty."), QMessageBox::Ok);
        }
        else {
            accept ();
        }
    });

    RoleModel *roles = new RoleModel();
    m_role->setModel (roles);

    QPushButton* cancel_btn = new QPushButton("Отмена");
    connect(cancel_btn, SIGNAL(clicked()), SLOT(reject()));

    QLabel* m_name_label = new QLabel("Введите имя");
    QLabel* m_email_label = new QLabel("Введите корректный email");
    QLabel* m_phone_label = new QLabel("Введите телефон");
    QLabel* m_role_label = new QLabel("Выберите роль");


    layout->addWidget (m_name_label);
    layout->addWidget (m_name);
    layout->addWidget (m_email_label);
    layout->addWidget (m_email);
    layout->addWidget (m_phone_label);
    layout->addWidget (m_phone);
    layout->addWidget (m_role_label);
    layout->addWidget (m_role);
    layout->addWidget(ok_btn);
    layout->addWidget(cancel_btn);

    setLayout( layout );
}

QString TWAddDialog::getName()
{
    return m_name->text ();
}

QString TWAddDialog::getEmail()
{
    return m_email->text ();
}

QString TWAddDialog::getPhone()
{
    return m_phone->text ();
}

QString TWAddDialog::getRole()
{
    return m_role->currentText ();
}

void TWAddDialog::adjustTextColor() {
    if(!m_email->hasAcceptableInput())
        m_email->setStyleSheet("QLineEdit { color: red;}");
    else
        m_email->setStyleSheet("QLineEdit { color: black;}");
}

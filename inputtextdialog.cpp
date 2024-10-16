#include "inputtextdialog.h"
#include <QBoxLayout>
#include <QPushButton>

InputTextDialog::InputTextDialog(QWidget *parent) : QDialog(parent)
{   
    setModal(true);
    QBoxLayout* layout = new QHBoxLayout;

    m_txt = new QLineEdit;
    layout->addWidget( m_txt );

    QPushButton* ok_btn = new QPushButton("Добавить");
    connect( ok_btn, SIGNAL(clicked()), SLOT(accept()));
    layout->addWidget(ok_btn);

    QPushButton* cancel_btn = new QPushButton("Отмена");
    connect(cancel_btn, SIGNAL(clicked()), SLOT(reject()));
    layout->addWidget(cancel_btn);

    setLayout( layout );
}

QString InputTextDialog::getText() const
{
    return m_txt->text();
}

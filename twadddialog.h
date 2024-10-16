#ifndef TWADDDIALOG_H
#define TWADDDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

class TWAddDialog : public QDialog
{
    Q_OBJECT
private:
    QLineEdit* m_name;
    QLineEdit* m_email;
    QLineEdit* m_phone;
    QComboBox* m_role;

public slots:
    void adjustTextColor();

public:
    TWAddDialog(QWidget *parent = nullptr);
    QString getName();
    QString getEmail();
    QString getPhone();
    QString getRole();
};

#endif // TWADDDIALOG_H

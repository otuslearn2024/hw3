#ifndef INPUTTEXTDIALOG_H
#define INPUTTEXTDIALOG_H
#include <QDialog>
#include <QObject>
#include <QLineEdit>


class InputTextDialog : public QDialog
{
    Q_OBJECT
private:
    QLineEdit* m_txt;
public:
    InputTextDialog(QWidget* patrnt = nullptr);
    QString getText() const;
};

#endif // INPUTTEXTDIALOG_H

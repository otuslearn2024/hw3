#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemSelectionModel>
#include <QStringListModel>
#include <QStringList>
#include <QObject>
#include <QDebug>
#include "inputtextdialog.h"
#include "dbmanager.h"
#include <QSqlTableModel>
#include "mytableviewmodel.h"
#include "twadddialog.h"
#include <QSqlRecord>
#include "twroleselectdelegate.h"
#include "golditem.h"
#include "mygoldmodel.h"
#include "golditemdelegate.h"

QStringList* lw_data_list;
QStringListModel* sl_model;
MyTableViewModel* twModel;
MyGoldModel* mgModel;
DBManager* dbManager;
QList<GoldItem> *goldItemList;
QLineSeries *sellSeries;
QLineSeries *buySeries;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbManager = new DBManager();
    dbManager->connectDatabase();

    //-------------------------------------------------------
    //модель для listview
    lw_data_list = new QStringList();
    for(int i = 0; i<10; i++) {
        QString item = QString("Элемент %1").arg(i);
        lw_data_list->append(item);
    }

    sl_model = new QStringListModel();
    sl_model->setStringList(*lw_data_list);
    ui->listView->setModel(sl_model);

    connect(ui->listView, &QListView::clicked, [&](const QModelIndex idx) {
        qDebug() << idx.row();
        ui->label_lw_selected->setText(idx.data(Qt::DisplayRole).value<QString>());
    });

    //кнопка удаления
    connect(ui->pushButton_lw_delete, &QPushButton::clicked, [&]() {
       QModelIndex idx = ui->listView->currentIndex();
       int row = idx.row();
       if(row >= 0) {
           sl_model->removeRow(row);
           idx = ui->listView->currentIndex();
           ui->label_lw_selected->setText(idx.data(Qt::DisplayRole).value<QString>());
       }
    });

    //кнопка добавления
    connect(ui->pushButton_lw_add, &QPushButton::clicked, [&]() {
        InputTextDialog *dlg = new InputTextDialog(this);
        switch (dlg->exec()) {
            case QDialog::Accepted:
                qDebug() << QString::fromUtf8("Added text: ") << dlg->getText();
                //плохой способ?
                //lw_data_list->append(QString(dlg->getText()));
                //sl_model->setStringList(*lw_data_list);

                sl_model->insertRow(sl_model->rowCount());
                sl_model->setData(sl_model->index(sl_model->rowCount() - 1, 0), dlg->getText());
                break;

            case QDialog::Rejected:
                qDebug() << "Canceled";
                break;

            default:
                break;
        }
    });
    //-------------------------------------------------------
    //TableView
    twModel = new MyTableViewModel(this, dbManager->getDb ());
    twModel->setTable(dbManager->getTableName());
    twModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    twModel->select();
    twModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    twModel->setHeaderData(1, Qt::Horizontal, tr("Имя"));
    twModel->setHeaderData(2, Qt::Horizontal, tr("Телефон"));
    twModel->setHeaderData(3, Qt::Horizontal, tr("Email"));
    twModel->setHeaderData(4, Qt::Horizontal, tr("Role"));
    //убрать номера строк
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setModel(twModel);

    TWRoleSelectDelegate *delegate = new TWRoleSelectDelegate();
    ui->tableView->setItemDelegateForColumn (4, delegate);

    //кнопка добавления
    connect(ui->pushButton_tw_add, &QPushButton::clicked, [&]() {
        TWAddDialog* dialog = new TWAddDialog(this);
        switch (dialog->exec ()) {
            case QDialog::Accepted:
                qDebug() << dialog->getName ();
                qDebug() << dialog->getRole ();
                qDebug() << dialog->getEmail ();
                qDebug() << dialog->getPhone ();
                {
                    QVariantList *data = new QVariantList();
                    data->append (dialog->getName ());
                    data->append (dialog->getPhone ());
                    data->append (dialog->getEmail ());
                    data->append (dialog->getRole ());
                    dbManager->insert (data);
                    twModel->updateModel ();
                }
                break;
            case QDialog::Rejected:
                break;
            default:
                break;
        }
    });
    //клик на строку
    connect(ui->tableView, &QTableView::clicked, [&](const QModelIndex idx) {
        int id = twModel->data(ui->tableView->model()->index(idx.row(),0)).toInt();
        QString record = dbManager->getPrintableRecord (id);
        ui->label_tw_selected->setText (record);
        qDebug() << id;
    });
    //удалить
    connect(ui->pushButton_tw_delete, &QPushButton::clicked, [&]() {
        QModelIndex idx = ui->tableView->currentIndex();
        int id = twModel->data(ui->tableView->model()->index(idx.row(),0)).toInt();
        dbManager->remove (id);
        twModel->updateModel ();
    });

    //=========================GOLD==============================
    QList<GoldItem>* tmp_GoldList = new QList<GoldItem>();
    GoldItem gi;
    gi.setDateTime (QDateTime::currentDateTime());
    gi.setPriceSell (213.22344444);
    gi.setPricePurchase (210.156546546);
    tmp_GoldList->append (gi);
    gi.setDateTime (QDateTime::currentDateTime());
    gi.setPriceSell (211.2);
    gi.setPricePurchase (209.1);
    tmp_GoldList->append (gi);
    gi.setDateTime (QDateTime::currentDateTime());
    gi.setPriceSell (215.2);
    gi.setPricePurchase (213.1);
    tmp_GoldList->append (gi);


    GoldItemDelegate *giDelegate = new GoldItemDelegate();
    ui->tableView_gold->setItemDelegateForColumn (1, giDelegate);
    ui->tableView_gold->setItemDelegateForColumn (2, giDelegate);

    //=====открыть и загрузить csv
    connect(ui->actionOpen_File, &QAction::triggered, [&]() {
        QString fileName = QFileDialog::getOpenFileName(this,
             "Open Gold Sales CSV file", "", "CSV Files (*.csv)");
        qDebug() << fileName;
        QFile file(fileName);
        if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
            QMessageBox::warning(this, tr("File open error"), "Error opening file", QMessageBox::Ok);
        } else {
            goldItemList = new QList<GoldItem>();
            sellSeries = new QLineSeries();
            buySeries = new QLineSeries();
            QTextStream in(&file);
            bool isFirstLine = true;
            while (!in.atEnd())
            {
                QString line = in.readLine();
                if(isFirstLine) {
                    isFirstLine = false;
                }
                else {
                    QStringList list = line.split(",");
                    if(list.size () != 3) {
                        QMessageBox::warning(this, tr("File open error"), "Incorrect csv", QMessageBox::Ok);
                        return;
                    }
                    GoldItem g;
                    g.setDateTime (QDateTime::fromString (list.at (0), "yyyy-MM-dd hh:mm:ss"));
                    g.setPriceSell (list.at (1).toFloat ());
                    g.setPricePurchase (list.at (2).toFloat ());
                    qDebug() << g.DateTime ().toMSecsSinceEpoch();
                    sellSeries->append (g.DateTime ().toMSecsSinceEpoch(), g.PriceSell ());
                    buySeries->append (g.DateTime ().toMSecsSinceEpoch(), g.PricePurchase ());
                    //qDebug() << g.DateTime ();
                    goldItemList->append (g);
                }
            }
            file.close();
            mgModel = new MyGoldModel(goldItemList);
            ui->tableView_gold->setModel (mgModel);
            ui->tableView_gold->verticalHeader()->hide();
            ui->tableView_gold->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

            QChart *chart = new QChart();
            chart->legend()->hide();
            chart->addSeries(sellSeries);
            chart->addSeries (buySeries);

            auto axisX = new QDateTimeAxis;
            axisX->setTickCount(20);
            axisX->setFormat("yyyy-MM-dd hh:mm");

            axisX->setTitleText("Datetime");
            chart->addAxis(axisX, Qt::AlignBottom);
            sellSeries->attachAxis(axisX);
            buySeries->attachAxis (axisX);

            auto axisY = new QValueAxis;
            axisY->setLabelFormat("%f");
            axisY->setTitleText("Price");
            chart->addAxis(axisY, Qt::AlignLeft);
            sellSeries->attachAxis(axisY);
            buySeries->attachAxis (axisY);

            //chart->createDefaultAxes();
            chart->setTitle("Gold prices");
            ui->graphicsView->setChart (chart);
        }
    });

    //======chart




}

MainWindow::~MainWindow()
{
    delete ui;
}


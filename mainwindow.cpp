#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAdd_clicked()
{
    int row = ui->tableItems->rowCount();

    ui->tableItems->insertRow(row);

    ui->tableItems->setItem(row, 0, new QTableWidgetItem("ITEM-001"));
    ui->tableItems->setItem(row, 1, new QTableWidgetItem("Milk"));
    ui->tableItems->setItem(row, 2, new QTableWidgetItem("Dairy"));
    ui->tableItems->setItem(row, 3, new QTableWidgetItem("4.99"));
    ui->tableItems->setItem(row, 4, new QTableWidgetItem("2"));
    ui->tableItems->setItem(row, 5, new QTableWidgetItem("9.98"));
}

void MainWindow::on_btnSave_clicked()
{
    manager.getItems().clear();

    for(int row = 0; row < ui->tableItems->rowCount(); row++)
    {
        QString id = ui->tableItems->item(row, 0)->text();
        QString name = ui->tableItems->item(row, 1)->text();
        QString category = ui->tableItems->item(row, 2)->text();
        QString price = ui->tableItems->item(row, 3)->text();
        QString quantity = ui->tableItems->item(row, 4)->text();
    }

}


void MainWindow::on_btnLoad_clicked()
{

}


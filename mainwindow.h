#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DataManager.h"
#include "Budget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_btnAdd_clicked();
    void on_btnSave_clicked();
    void on_btnLoad_clicked();

private:
    Ui::MainWindow *ui;
    DataManager manager;
    Budget budget;
};
#endif // MAINWINDOW_H

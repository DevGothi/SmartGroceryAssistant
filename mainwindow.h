#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DataManager.h"
#include "ItemCrud.h"

#include <QMainWindow>
#include <QString>

#include <string>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class QCloseEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onAddItem();
    void onEditItem();
    void onDeleteItem();
    void onClearForm();
    void onSearchTextChanged(const QString& text);
    void onTableCellClicked(int row, int column);
    void onSaveData();
    void onLoadData();
    void onOpenHelp();
    void onExitApplication();
    void validateFormLive();

private:
    Ui::MainWindow* ui;
    DataManager dataManager;
    std::string selectedItemID;
    QString currentFilePath;
    bool hasUnsavedChanges;

    void setupConnections();
    void setupTable();
    void loadStartupData();

    GroceryItemInput readItemForm() const;
    void populateTable(const std::vector<GroceryItem>& items);
    void refreshVisibleItems();
    void clearItemForm();
    void showOperationResult(const ItemOperationResult& result);
    void markDataChanged();

    bool saveUsingDialog();
    bool saveToFile(const QString& filePath);
    bool maybeSaveChanges();
};

#endif // MAINWINDOW_H
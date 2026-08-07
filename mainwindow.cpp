#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "HelpDialog.h"
#include "ItemSearchDelete.h"
#include "ItemValidation.h"

#include <QAbstractItemView>
#include <QPalette>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidgetItem>

#include <algorithm>
#include <exception>
#include <optional>

namespace
{
    bool containsText(const std::vector<std::string>& errors,
        const std::string& text)
    {
        return std::any_of(
            errors.begin(),
            errors.end(),
            [&](const std::string& error)
            {
                return error.find(text) != std::string::npos;
            });
    }

    void setFieldError(QWidget* widget, bool hasError)
    {
        if (hasError)
        {
            widget->setStyleSheet(
                "border: 1px solid #c62828;"
                "background-color: #fff5f5;"
                "color: black;");
        }
        else
        {
            widget->setStyleSheet(
                "color: black;"
                "background-color: white;");
        }
    }
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    hasUnsavedChanges(false)
{
    ui->setupUi(this);

    ui->categoryBox->setStyleSheet(
        "QComboBox {"
        "   color: black;"
        "   background-color: white;"
        "}"
        "QComboBox QAbstractItemView {"
        "   color: black;"
        "   background-color: white;"
        "   selection-color: black;"
        "   selection-background-color: #1f7bd1;"
        "}"
        );

    ui->itemsTable->setStyleSheet(
        "QTableWidget {"
        "    color: #000000;"
        "    background-color: #ffffff;"
        "}"
        "QTableWidget::item {"
        "    color: #000000;"
        "}"
        "QTableWidget::item:selected {"
        "    color: #ffffff;"
        "    background-color: #a64ac9;"
        "}"
        );

    setupTable();
    setupConnections();
    loadStartupData();
    clearItemForm();

    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTable()
{
    ui->itemsTable->setColumnCount(6);
    ui->itemsTable->setHorizontalHeaderLabels(
        { "Item ID", "Name", "Category", "Price", "Quantity", "Total Cost" });

    ui->itemsTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);

    ui->itemsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->itemsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->itemsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->itemsTable->setAlternatingRowColors(true);
    ui->itemsTable->setSortingEnabled(true);
}

void MainWindow::setupConnections()
{
    connect(ui->addButton,
        &QPushButton::clicked,
        this,
        &MainWindow::onAddItem);

    connect(ui->editButton,
        &QPushButton::clicked,
        this,
        &MainWindow::onEditItem);

    connect(ui->deleteButton,
        &QPushButton::clicked,
        this,
        &MainWindow::onDeleteItem);

    connect(ui->clearFormButton,
        &QPushButton::clicked,
        this,
        &MainWindow::onClearForm);

    connect(ui->clearSearchButton,
        &QPushButton::clicked,
        ui->searchEdit,
        &QLineEdit::clear);

    connect(ui->searchEdit,
        &QLineEdit::textChanged,
        this,
        &MainWindow::onSearchTextChanged);

    connect(ui->itemsTable,
        &QTableWidget::cellClicked,
        this,
        &MainWindow::onTableCellClicked);

    connect(ui->actionSaveData,
        &QAction::triggered,
        this,
        &MainWindow::onSaveData);

    connect(ui->actionLoadData,
        &QAction::triggered,
        this,
        &MainWindow::onLoadData);

    connect(ui->actionExit,
        &QAction::triggered,
        this,
        &MainWindow::onExitApplication);

    connect(ui->actionHelp,
        &QAction::triggered,
        this,
        &MainWindow::onOpenHelp);

    connect(ui->itemIdEdit,
        &QLineEdit::textChanged,
        this,
        &MainWindow::validateFormLive);

    connect(ui->itemNameEdit,
        &QLineEdit::textChanged,
        this,
        &MainWindow::validateFormLive);

    connect(ui->priceEdit,
        &QLineEdit::textChanged,
        this,
        &MainWindow::validateFormLive);

    connect(ui->quantityEdit,
        &QLineEdit::textChanged,
        this,
        &MainWindow::validateFormLive);

    connect(ui->categoryBox,
        &QComboBox::currentTextChanged,
        this,
        &MainWindow::validateFormLive);
}

void MainWindow::loadStartupData()
{
    currentFilePath = QDir::current().filePath("data/grocery_data.json");

    if (QFileInfo::exists(currentFilePath))
    {
        try
        {
            if (dataManager.loadData(currentFilePath.toStdString()))
            {
                refreshVisibleItems();
                hasUnsavedChanges = false;
                statusBar()->showMessage("Previous grocery data loaded.", 4000);
                return;
            }
        }
        catch (const std::exception& exception)
        {
            QMessageBox::warning(
                this,
                "Startup File Warning",
                "The saved grocery file could not be loaded. Sample data will be used.\n\n"
                + QString::fromStdString(exception.what()));
        }
    }

    dataManager.loadSampleData();
    refreshVisibleItems();
    hasUnsavedChanges = false;
}

GroceryItemInput MainWindow::readItemForm() const
{
    return {
        ui->itemIdEdit->text().toStdString(),
        ui->itemNameEdit->text().toStdString(),
        ui->categoryBox->currentText().toStdString(),
        ui->priceEdit->text().toStdString(),
        ui->quantityEdit->text().toStdString() };
}

void MainWindow::populateTable(const std::vector<GroceryItem>& items)
{
    ui->itemsTable->setSortingEnabled(false);
    ui->itemsTable->clearContents();
    ui->itemsTable->setRowCount(static_cast<int>(items.size()));

    for (int row = 0; row < static_cast<int>(items.size()); ++row)
    {
        const GroceryItem& item = items[static_cast<std::size_t>(row)];

        ui->itemsTable->setItem(
            row,
            0,
            new QTableWidgetItem(
                QString::fromStdString(item.getItemID())));

        ui->itemsTable->setItem(
            row,
            1,
            new QTableWidgetItem(
                QString::fromStdString(item.getName())));

        ui->itemsTable->setItem(
            row,
            2,
            new QTableWidgetItem(
                QString::fromStdString(item.getCategory())));

        ui->itemsTable->setItem(
            row,
            3,
            new QTableWidgetItem(
                QString::number(item.getPrice(), 'f', 2)));

        ui->itemsTable->setItem(
            row,
            4,
            new QTableWidgetItem(
                QString::number(item.getQuantity())));

        ui->itemsTable->setItem(
            row,
            5,
            new QTableWidgetItem(
                QString::number(item.getCost(), 'f', 2)));
    }

    ui->itemsTable->setSortingEnabled(true);
}

void MainWindow::refreshVisibleItems()
{
    const std::vector<GroceryItem> results = ::searchItems(
        dataManager.getItems(),
        ui->searchEdit->text().toStdString());

    populateTable(results);

    statusBar()->showMessage(
        QString("%1 grocery item(s) shown").arg(results.size()));
}

void MainWindow::clearItemForm()
{
    selectedItemID.clear();

    ui->itemIdEdit->clear();
    ui->itemNameEdit->clear();
    ui->categoryBox->setCurrentIndex(0);
    ui->priceEdit->clear();
    ui->quantityEdit->clear();
    ui->itemsTable->clearSelection();

    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);

    ui->formValidationLabel->clear();
    ui->formValidationLabel->setStyleSheet("");

    setFieldError(ui->itemIdEdit, false);
    setFieldError(ui->itemNameEdit, false);
    setFieldError(ui->categoryBox, false);
    setFieldError(ui->priceEdit, false);
    setFieldError(ui->quantityEdit, false);

    ui->itemIdEdit->setFocus();
}

void MainWindow::showOperationResult(const ItemOperationResult& result)
{
    if (result.success)
    {
        QMessageBox::information(
            this,
            "Success",
            QString::fromStdString(result.message));
        return;
    }

    QString errorMessage;

    for (const std::string& error : result.errors)
    {
        errorMessage += "• " + QString::fromStdString(error) + "\n";
    }

    if (errorMessage.isEmpty())
    {
        errorMessage = QString::fromStdString(result.message);
    }

    QMessageBox::warning(
        this,
        "Grocery Item Error",
        errorMessage.trimmed());
}

void MainWindow::markDataChanged()
{
    hasUnsavedChanges = true;
    setWindowModified(true);
    statusBar()->showMessage("Unsaved changes", 3000);
}

void MainWindow::onAddItem()
{
    const ItemOperationResult result = addGroceryItem(
        dataManager.getItems(),
        readItemForm());

    showOperationResult(result);

    if (result.success)
    {
        markDataChanged();
        clearItemForm();
        refreshVisibleItems();
    }
}

void MainWindow::onEditItem()
{
    if (selectedItemID.empty())
    {
        QMessageBox::warning(
            this,
            "Edit Grocery Item",
            "Select an item from the table before pressing Edit.");
        return;
    }

    const ItemOperationResult result = editGroceryItem(
        dataManager.getItems(),
        selectedItemID,
        readItemForm());

    showOperationResult(result);

    if (result.success)
    {
        markDataChanged();
        clearItemForm();
        refreshVisibleItems();
    }
}

void MainWindow::onDeleteItem()
{
    if (selectedItemID.empty())
    {
        QMessageBox::warning(
            this,
            "Delete Grocery Item",
            "Select an item from the table before pressing Delete.");
        return;
    }

    const QMessageBox::StandardButton answer = QMessageBox::question(
        this,
        "Confirm Delete",
        "Delete item " + QString::fromStdString(selectedItemID) + "?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (answer != QMessageBox::Yes)
    {
        return;
    }

    const ItemOperationResult result = removeGroceryItem(
        dataManager.getItems(),
        selectedItemID);

    showOperationResult(result);

    if (result.success)
    {
        markDataChanged();
        clearItemForm();
        refreshVisibleItems();
    }
}

void MainWindow::onClearForm()
{
    clearItemForm();
}

void MainWindow::onSearchTextChanged(const QString& text)
{
    Q_UNUSED(text);

    selectedItemID.clear();
    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->itemsTable->clearSelection();

    refreshVisibleItems();
}

void MainWindow::onTableCellClicked(int row, int column)
{
    Q_UNUSED(column);

    QTableWidgetItem* idCell = ui->itemsTable->item(row, 0);

    if (idCell == nullptr)
    {
        return;
    }

    selectedItemID = idCell->text().toStdString();

    const std::optional<std::size_t> index = findItemIndexByID(
        dataManager.getItems(),
        selectedItemID);

    if (!index.has_value())
    {
        selectedItemID.clear();
        return;
    }

    const GroceryItem& item = dataManager.getItems()[*index];

    ui->itemIdEdit->setText(
        QString::fromStdString(item.getItemID()));

    ui->itemNameEdit->setText(
        QString::fromStdString(item.getName()));

    ui->categoryBox->setCurrentText(
        QString::fromStdString(item.getCategory()));

    ui->priceEdit->setText(
        QString::number(item.getPrice(), 'f', 2));

    ui->quantityEdit->setText(
        QString::number(item.getQuantity()));

    ui->editButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);

    validateFormLive();
}

void MainWindow::validateFormLive()
{
    const bool formIsBlank =
        ui->itemIdEdit->text().trimmed().isEmpty() &&
        ui->itemNameEdit->text().trimmed().isEmpty() &&
        ui->priceEdit->text().trimmed().isEmpty() &&
        ui->quantityEdit->text().trimmed().isEmpty() &&
        ui->categoryBox->currentIndex() == 0;

    if (formIsBlank)
    {
        ui->formValidationLabel->clear();

        setFieldError(ui->itemIdEdit, false);
        setFieldError(ui->itemNameEdit, false);
        setFieldError(ui->categoryBox, false);
        setFieldError(ui->priceEdit, false);
        setFieldError(ui->quantityEdit, false);
        return;
    }

    const std::optional<std::string> ignoredItemID =
        selectedItemID.empty()
        ? std::nullopt
        : std::optional<std::string>(selectedItemID);

    const ItemValidationResult validation = validateGroceryItem(
        readItemForm(),
        dataManager.getItems(),
        ignoredItemID);

    if (validation.isValid())
    {
        ui->formValidationLabel->setText("Form is valid.");
        ui->formValidationLabel->setStyleSheet("color: #2e7d32;");

        setFieldError(ui->itemIdEdit, false);
        setFieldError(ui->itemNameEdit, false);
        setFieldError(ui->categoryBox, false);
        setFieldError(ui->priceEdit, false);
        setFieldError(ui->quantityEdit, false);
        return;
    }

    QString message;
    for (const std::string& error : validation.errors)
    {
        message += "• " + QString::fromStdString(error) + "\n";
    }

    ui->formValidationLabel->setText(message.trimmed());
    ui->formValidationLabel->setStyleSheet("color: #c62828;");

    setFieldError(
        ui->itemIdEdit,
        containsText(validation.errors, "Item ID"));

    setFieldError(
        ui->itemNameEdit,
        containsText(validation.errors, "Item name"));

    setFieldError(
        ui->categoryBox,
        containsText(validation.errors, "Category"));

    setFieldError(
        ui->priceEdit,
        containsText(validation.errors, "Price"));

    setFieldError(
        ui->quantityEdit,
        containsText(validation.errors, "Quantity"));
}

bool MainWindow::saveToFile(const QString& filePath)
{
    if (filePath.trimmed().isEmpty())
    {
        return false;
    }

    QFileInfo fileInfo(filePath);
    QDir().mkpath(fileInfo.absolutePath());

    try
    {
        if (!dataManager.saveData(filePath.toStdString()))
        {
            QMessageBox::critical(
                this,
                "Save Failed",
                "The selected file could not be opened for writing.");
            return false;
        }
    }
    catch (const std::exception& exception)
    {
        QMessageBox::critical(
            this,
            "Save Failed",
            QString::fromStdString(exception.what()));
        return false;
    }

    currentFilePath = filePath;
    hasUnsavedChanges = false;
    setWindowModified(false);

    statusBar()->showMessage("Application data saved to JSON.", 4000);
    return true;
}

bool MainWindow::saveUsingDialog()
{
    const QString selectedPath = QFileDialog::getSaveFileName(
        this,
        "Save Grocery Data",
        currentFilePath,
        "JSON files (*.json);;All files (*.*)");

    if (selectedPath.isEmpty())
    {
        return false;
    }

    return saveToFile(selectedPath);
}

void MainWindow::onSaveData()
{
    saveUsingDialog();
}

bool MainWindow::maybeSaveChanges()
{
    if (!hasUnsavedChanges)
    {
        return true;
    }

    const QMessageBox::StandardButton answer = QMessageBox::warning(
        this,
        "Unsaved Changes",
        "You have unsaved changes. Do you want to save before continuing?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);

    if (answer == QMessageBox::Cancel)
    {
        return false;
    }

    if (answer == QMessageBox::Save)
    {
        return saveUsingDialog();
    }

    return true;
}

void MainWindow::onLoadData()
{
    if (!maybeSaveChanges())
    {
        return;
    }

    const QString selectedPath = QFileDialog::getOpenFileName(
        this,
        "Load Grocery Data",
        currentFilePath,
        "JSON files (*.json);;All files (*.*)");

    if (selectedPath.isEmpty())
    {
        return;
    }

    try
    {
        if (!dataManager.loadData(selectedPath.toStdString()))
        {
            QMessageBox::critical(
                this,
                "Load Failed",
               "The file could not be loaded. It may be missing, empty, corrupted, or contain invalid data.");
            return;
        }
    }
    catch (const std::exception& exception)
    {
        QMessageBox::critical(
            this,
            "Load Failed",
            "The file contains invalid or corrupted grocery data.\n\n"
            + QString::fromStdString(exception.what()));
        return;
    }

    currentFilePath = selectedPath;
    hasUnsavedChanges = false;
    setWindowModified(false);

    ui->searchEdit->clear();
    clearItemForm();
    refreshVisibleItems();

    statusBar()->showMessage("Application data loaded from JSON.", 4000);
}

void MainWindow::onOpenHelp()
{
    HelpDialog helpDialog(this);
    helpDialog.exec();
}

void MainWindow::onExitApplication()
{
    close();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (maybeSaveChanges())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
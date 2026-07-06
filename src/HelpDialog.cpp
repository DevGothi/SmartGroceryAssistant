#include "HelpDialog.h"

#include <QVBoxLayout>
#include <QTextBrowser>
#include <QPushButton>

HelpDialog::HelpDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Help - Smart Grocery Shopping Assistant");
    resize(500, 400);

    auto* layout = new QVBoxLayout(this);

    auto* text = new QTextBrowser(this);
    text->setOpenExternalLinks(true);
    text->setHtml(R"(
        <h2>Smart Grocery Shopping Assistant - Help</h2>
        <p>Welcome! Here is how to use the main features.</p>

        <h3>Items</h3>
        <ul>
          <li><b>Add:</b> Fill in the Item ID, Name, Category, Price and
              Quantity, then click <b>Save</b>.</li>
          <li><b>Delete:</b> Select an item in the table and click
              <b>Delete</b>. If no item is selected you will be warned.</li>
          <li><b>Search:</b> Type in the search box to filter items by name or
              category. Search is case-insensitive. Leave it empty to show all
              items.</li>
        </ul>

        <h3>Shopping Lists</h3>
        <p>Create a list, add grocery items to it, update quantities, and mark
           the list as completed when done.</p>

        <h3>Budget</h3>
        <p>Set a budget limit. The app warns you when your total spending gets
           close to or exceeds the limit.</p>

        <h3>Analytics</h3>
        <p>View total spending, spending by category, most purchased items and
           average item cost.</p>

        <hr>
        <p><i>CST8219 - C++ Programming group project.</i></p>
    )");

    auto* closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    layout->addWidget(text);
    layout->addWidget(closeButton);
}
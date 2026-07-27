#include "Budget.h"
#include "DataManager.h"
#include "GroceryItem.h"
#include "ItemCrud.h"
#include "ItemSearchDelete.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace
{
    void printItems(const std::vector<GroceryItem>& items)
    {
        if (items.empty())
        {
            std::cout << "No items found.\n";
            return;
        }

        std::cout
            << std::left
            << std::setw(12) << "ID"
            << std::setw(22) << "Name"
            << std::setw(12) << "Category"
            << std::right
            << std::setw(10) << "Price"
            << std::setw(8) << "Qty"
            << std::setw(12) << "Cost"
            << '\n';

        std::cout << std::string(76, '-') << '\n';

        for (const auto& item : items)
        {
            std::cout
                << std::left
                << std::setw(12) << item.getItemID()
                << std::setw(22) << item.getName()
                << std::setw(12) << item.getCategory()
                << std::right
                << std::fixed
                << std::setprecision(2)
                << std::setw(10) << item.getPrice()
                << std::setw(8) << item.getQuantity()
                << std::setw(12) << item.getCost()
                << '\n';
        }
    }

    std::string ask(const std::string& prompt)
    {
        std::cout << prompt;

        std::string answer;
        std::getline(std::cin, answer);

        return answer;
    }

    GroceryItemInput readItemInput()
    {
        GroceryItemInput input;

        input.itemID =
            ask("Item ID (example ITEM-0001): ");

        input.name =
            ask("Item name: ");

        input.category =
            ask("Category (Produce/Dairy/Meat/Bakery/Pantry): ");

        input.price =
            ask("Price: ");

        input.quantity =
            ask("Quantity: ");

        return input;
    }

    void printOperationResult(
        const ItemOperationResult& result
    )
    {
        if (result.success)
        {
            std::cout
                << "Success: "
                << result.message
                << '\n';

            return;
        }

        std::cout
            << "Error: "
            << result.message
            << '\n';

        for (const auto& error : result.errors)
        {
            std::cout
                << "- "
                << error
                << '\n';
        }
    }
}

int main()
{
    DataManager dataManager;
    dataManager.loadSampleData();

    std::vector<GroceryItem>& items =
        dataManager.getItems();

    Budget budget;
    budget.setBudget(50.00);

    std::cout
        << "============================================\n"
        << " Smart Grocery Shopping Assistant\n"
        << " Grocery Item CRUD Demonstration\n"
        << "============================================\n";

    while (true)
    {
        std::cout
            << "\nMenu:\n"
            << "1 - Show all items\n"
            << "2 - Add grocery item\n"
            << "3 - Edit grocery item\n"
            << "4 - Search items by name or category\n"
            << "5 - Delete an item by ID\n"
            << "6 - Check budget\n"
            << "7 - Save data\n"
            << "8 - Load data\n"
            << "9 - Exit\n";

        const std::string choice =
            ask("Choose an option: ");

        if (choice == "1")
        {
            printItems(items);
        }
        else if (choice == "2")
        {
            std::cout << "\nAdd Grocery Item\n";

            const GroceryItemInput input =
                readItemInput();

            const ItemOperationResult result =
                addGroceryItem(items, input);

            printOperationResult(result);

            if (result.success)
            {
                printItems(items);
            }
        }
        else if (choice == "3")
        {
            std::cout << "\nEdit Grocery Item\n";

            const std::string originalID =
                ask("Enter the current Item ID: ");

            if (!findItemIndexByID(
                items,
                originalID
            ).has_value())
            {
                std::cout
                    << "Error: Item was not found.\n";

                continue;
            }

            std::cout
                << "Enter all updated information.\n"
                << "The Item ID may remain the same.\n";

            const GroceryItemInput updatedInput =
                readItemInput();

            const ItemOperationResult result =
                editGroceryItem(
                    items,
                    originalID,
                    updatedInput
                );

            printOperationResult(result);

            if (result.success)
            {
                printItems(items);
            }
        }
        else if (choice == "4")
        {
            const std::string query =
                ask("Enter item name or category: ");

            const std::vector<GroceryItem> results =
                searchItems(items, query);

            printItems(results);
        }
        else if (choice == "5")
        {
            const std::string itemID =
                ask("Enter the Item ID to delete: ");

            const ItemOperationResult result =
                removeGroceryItem(items, itemID);

            printOperationResult(result);

            if (result.success)
            {
                printItems(items);
            }
        }
        else if (choice == "6")
        {
            const double total =
                budget.calculateTotalCost(items);

            std::cout
                << std::fixed
                << std::setprecision(2)
                << "Budget limit: $"
                << budget.getBudget()
                << '\n'
                << "Current total: $"
                << total
                << '\n'
                << "Remaining budget: $"
                << budget.getRemainingBudget(items)
                << '\n';

            if (budget.isOverBudget(items))
            {
                std::cout
                    << "Warning: You are over budget!\n";
            }
            else
            {
                std::cout
                    << "You are within budget.\n";
            }
        }
        else if (choice == "7")
        {
            if (dataManager.saveData("grocery.txt"))
            {
                std::cout
                    << "Data saved to grocery.txt.\n";
            }
            else
            {
                std::cout
                    << "Error: Data could not be saved.\n";
            }
        }
        else if (choice == "8")
        {
            try
            {
                if (dataManager.loadData("grocery.txt"))
                {
                    std::cout
                        << "Data loaded from grocery.txt.\n";

                    printItems(items);
                }
                else
                {
                    std::cout
                        << "Error: grocery.txt could not be opened.\n";
                }
            }
            catch (const std::exception& exception)
            {
                std::cout
                    << "Error while loading data: "
                    << exception.what()
                    << '\n';
            }
        }
        else if (choice == "9")
        {
            std::cout << "Goodbye!\n";
            break;
        }
        else
        {
            std::cout
                << "Invalid option. Enter a number from 1 to 9.\n";
        }
    }

    return 0;
}
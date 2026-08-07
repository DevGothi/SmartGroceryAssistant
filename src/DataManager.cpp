#include "DataManager.h"

#include "GroceryManager.h"

#include <fstream>
#include <sstream>
#include <string>

bool DataManager::saveData(
    const GroceryManager& groceryManager,
    const std::string& filename
) const
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    for (const auto& item : groceryManager.getItems())
    {
        file << item.getItemID() << ","
             << item.getName() << ","
             << item.getCategory() << ","
             << item.getPrice() << ","
             << item.getQuantity() << "\n";
    }

    return true;
}

bool DataManager::loadData(
    GroceryManager& groceryManager,
    const std::string& filename
) const
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    groceryManager.getItems().clear();

    std::string line;

    try
    {
        while (std::getline(file, line))
        {
            if (line.empty())
            {
                continue;
            }

            std::stringstream stream(line);

            std::string itemID;
            std::string name;
            std::string category;
            std::string priceText;
            std::string quantityText;

            std::getline(stream, itemID, ',');
            std::getline(stream, name, ',');
            std::getline(stream, category, ',');
            std::getline(stream, priceText, ',');
            std::getline(stream, quantityText);

            if (
                itemID.empty() ||
                name.empty() ||
                category.empty() ||
                priceText.empty() ||
                quantityText.empty()
            )
            {
                return false;
            }

            const double price = std::stod(priceText);
            const int quantity = std::stoi(quantityText);

            GroceryItem item(
                itemID,
                name,
                category,
                price,
                quantity
            );

            if (!groceryManager.addItem(item))
            {
                return false;
            }
        }
    }
    catch (const std::exception&)
    {
        groceryManager.getItems().clear();
        return false;
    }

    return true;
}
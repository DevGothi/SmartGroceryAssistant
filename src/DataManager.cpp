#include "DataManager.h"
#include <fstream>
#include <sstream>

void DataManager::addItem(const GroceryItem& item)
{
    items.push_back(item);
}

std::vector<GroceryItem>& DataManager::getItems()
{
    return items;
}

const std::vector<GroceryItem>& DataManager::getItems() const
{
    return items;
}

void DataManager::loadSampleData()
{
    items.clear();

    items.push_back(GroceryItem("ITEM-0001", "Milk", "Dairy", 3.49, 2));
    items.push_back(GroceryItem("ITEM-0002", "Cheddar", "Dairy", 6.99, 1));
    items.push_back(GroceryItem("ITEM-0003", "Apple", "Produce", 0.80, 12));
    items.push_back(GroceryItem("ITEM-0004", "Chicken", "Meat", 9.50, 1));
    items.push_back(GroceryItem("ITEM-0005", "Bread", "Bakery", 2.25, 3));
}

bool DataManager::saveData(const std::string& filename) const
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    for (const auto& item : items)
    {
        file << item.getItemID() << ","
             << item.getName() << ","
             << item.getCategory() << ","
             << item.getPrice() << ","
             << item.getQuantity() << "\n";
    }

    file.close();
    return true;
}

bool DataManager::loadData(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    items.clear();

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string id;
        std::string name;
        std::string category;
        std::string priceText;
        std::string quantityText;

        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, category, ',');
        std::getline(ss, priceText, ',');
        std::getline(ss, quantityText, ',');

        double price = std::stod(priceText);
        int quantity = std::stoi(quantityText);

        items.push_back(GroceryItem(id, name, category, price, quantity));
    }

    file.close();
    return true;
}
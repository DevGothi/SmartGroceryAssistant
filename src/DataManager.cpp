#include "DataManager.h"

#include "ItemValidation.h"

#include <fstream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace
{
    json groceryItemToJson(const GroceryItem& item)
    {
        return {
            {"itemID", item.getItemID()},
            {"name", item.getName()},
            {"category", item.getCategory()},
            {"price", item.getPrice()},
            {"quantity", item.getQuantity()}
        };
    }

    json shoppingListToJson(const ShoppingList& shoppingList)
    {
        json entries = json::array();

        for (const auto& entry : shoppingList.getItems())
        {
            entries.push_back({
                {"itemID", entry.first},
                {"quantity", entry.second}
            });
        }

        return {
            {"listID", shoppingList.getListID()},
            {"completed", shoppingList.isCompleted()},
            {"items", entries}
        };
    }

    json budgetToJson(const Budget& budget)
    {
        return {
            {"budgetID", budget.getBudgetID()},
            {"limit", budget.getBudget()}
        };
    }

    json userToJson(const UserProfile& user)
    {
        return {
            {"userID", user.getUserID()},
            {"name", user.getName()},
            {"budgetLimit", user.getBudgetLimit()}
        };
    }

    json purchaseToJson(const PurchaseRecord& purchase)
    {
        return {
            {"purchaseID", purchase.getPurchaseID()},
            {"shoppingListID", purchase.getShoppingListID()},
            {"purchaseDate", purchase.getPurchaseDate()},
            {"totalSpent", purchase.getTotalSpent()}
        };
    }

    bool hasRequiredTopLevelArrays(const json& document)
    {
        return document.is_object() &&
               document.contains("items") &&
               document.contains("shoppingLists") &&
               document.contains("budgets") &&
               document.contains("users") &&
               document.contains("purchases") &&
               document["items"].is_array() &&
               document["shoppingLists"].is_array() &&
               document["budgets"].is_array() &&
               document["users"].is_array() &&
               document["purchases"].is_array();
    }

    bool loadItems(
        const json& itemArray,
        GroceryManager& temporaryManager
    )
    {
        for (const json& itemJson : itemArray)
        {
            if (
                !itemJson.is_object() ||
                !itemJson.contains("itemID") ||
                !itemJson.contains("name") ||
                !itemJson.contains("category") ||
                !itemJson.contains("price") ||
                !itemJson.contains("quantity")
            )
            {
                return false;
            }

            GroceryItemInput input {
                itemJson.at("itemID").get<std::string>(),
                itemJson.at("name").get<std::string>(),
                itemJson.at("category").get<std::string>(),
                std::to_string(itemJson.at("price").get<double>()),
                std::to_string(itemJson.at("quantity").get<int>())
            };

            const ItemValidationResult validation =
                validateGroceryItem(
                    input,
                    temporaryManager.getItems()
                );

            if (!validation.isValid())
            {
                return false;
            }

            const auto& data = *validation.data;

            if (
                !temporaryManager.addItem(
                    GroceryItem(
                        data.itemID,
                        data.name,
                        data.category,
                        data.price,
                        data.quantity
                    )
                )
            )
            {
                return false;
            }
        }

        return true;
    }

    bool loadShoppingLists(
        const json& listArray,
        GroceryManager& temporaryManager
    )
    {
        for (const json& listJson : listArray)
        {
            if (
                !listJson.is_object() ||
                !listJson.contains("listID") ||
                !listJson.contains("completed") ||
                !listJson.contains("items") ||
                !listJson.at("items").is_array()
            )
            {
                return false;
            }

            const std::string listID =
                listJson.at("listID").get<std::string>();

            const bool completed =
                listJson.at("completed").get<bool>();

            ShoppingList shoppingList(listID, false);

            for (const json& entry : listJson.at("items"))
            {
                if (
                    !entry.is_object() ||
                    !entry.contains("itemID") ||
                    !entry.contains("quantity")
                )
                {
                    return false;
                }

                const std::string itemID =
                    entry.at("itemID").get<std::string>();

                const int quantity =
                    entry.at("quantity").get<int>();

                if (
                    !temporaryManager.itemIDExists(itemID) ||
                    quantity <= 0 ||
                    quantity > 10000 ||
                    !shoppingList.addItem(itemID, quantity)
                )
                {
                    return false;
                }
            }

            if (completed)
            {
                shoppingList.markCompleted();
            }

            if (
                !temporaryManager.addShoppingList(
                    shoppingList
                )
            )
            {
                return false;
            }
        }

        return true;
    }

    bool loadBudgets(
        const json& budgetArray,
        GroceryManager& temporaryManager
    )
    {
        for (const json& budgetJson : budgetArray)
        {
            if (
                !budgetJson.is_object() ||
                !budgetJson.contains("budgetID") ||
                !budgetJson.contains("limit")
            )
            {
                return false;
            }

            Budget budget(
                budgetJson.at("budgetID").get<std::string>(),
                budgetJson.at("limit").get<double>()
            );

            if (!temporaryManager.addBudget(budget))
            {
                return false;
            }
        }

        return true;
    }

    bool loadUsers(
        const json& userArray,
        GroceryManager& temporaryManager
    )
    {
        for (const json& userJson : userArray)
        {
            if (
                !userJson.is_object() ||
                !userJson.contains("userID") ||
                !userJson.contains("name") ||
                !userJson.contains("budgetLimit")
            )
            {
                return false;
            }

            UserProfile user(
                userJson.at("userID").get<std::string>(),
                userJson.at("name").get<std::string>(),
                userJson.at("budgetLimit").get<double>()
            );

            if (!temporaryManager.addUser(user))
            {
                return false;
            }
        }

        return true;
    }

    bool loadPurchases(
        const json& purchaseArray,
        GroceryManager& temporaryManager
    )
    {
        for (const json& purchaseJson : purchaseArray)
        {
            if (
                !purchaseJson.is_object() ||
                !purchaseJson.contains("purchaseID") ||
                !purchaseJson.contains("shoppingListID") ||
                !purchaseJson.contains("purchaseDate") ||
                !purchaseJson.contains("totalSpent")
            )
            {
                return false;
            }

            PurchaseRecord purchase(
                purchaseJson.at("purchaseID").get<std::string>(),
                purchaseJson.at("shoppingListID").get<std::string>(),
                purchaseJson.at("purchaseDate").get<std::string>(),
                purchaseJson.at("totalSpent").get<double>()
            );

            if (!temporaryManager.addPurchase(purchase))
            {
                return false;
            }
        }

        return true;
    }

    bool loadJsonFile(
        GroceryManager& destination,
        const std::string& filename
    )
    {
        std::ifstream file(filename);

        if (!file.is_open())
        {
            return false;
        }

        const std::string contents{
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        };

        if (contents.empty())
        {
            return false;
        }

        try
        {
            const json document = json::parse(contents);

            if (!hasRequiredTopLevelArrays(document))
            {
                return false;
            }

            GroceryManager temporaryManager;

            if (
                !loadItems(
                    document.at("items"),
                    temporaryManager
                ) ||
                !loadShoppingLists(
                    document.at("shoppingLists"),
                    temporaryManager
                ) ||
                !loadBudgets(
                    document.at("budgets"),
                    temporaryManager
                ) ||
                !loadUsers(
                    document.at("users"),
                    temporaryManager
                ) ||
                !loadPurchases(
                    document.at("purchases"),
                    temporaryManager
                )
            )
            {
                return false;
            }

            // Existing data is replaced only after every section is valid.
            destination = std::move(temporaryManager);
            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }
}

GroceryManager& DataManager::getGroceryManager()
{
    return groceryManager;
}

const GroceryManager&
DataManager::getGroceryManager() const
{
    return groceryManager;
}

std::vector<GroceryItem>& DataManager::getItems()
{
    return groceryManager.getItems();
}

const std::vector<GroceryItem>&
DataManager::getItems() const
{
    return groceryManager.getItems();
}

void DataManager::loadSampleData()
{
    groceryManager.clear();

    groceryManager.addItem(
        GroceryItem(
            "ITEM-0001",
            "Milk",
            "Dairy",
            3.49,
            2
        )
    );

    groceryManager.addItem(
        GroceryItem(
            "ITEM-0002",
            "Cheddar",
            "Dairy",
            6.99,
            1
        )
    );

    groceryManager.addItem(
        GroceryItem(
            "ITEM-0003",
            "Apple",
            "Produce",
            0.80,
            12
        )
    );

    groceryManager.addItem(
        GroceryItem(
            "ITEM-0004",
            "Chicken",
            "Meat",
            9.50,
            1
        )
    );

    groceryManager.addItem(
        GroceryItem(
            "ITEM-0005",
            "Bread",
            "Bakery",
            2.25,
            3
        )
    );
}

bool DataManager::saveData(
    const std::string& filename
) const
{
    return saveData(groceryManager, filename);
}

bool DataManager::loadData(
    const std::string& filename
)
{
    return loadJsonFile(
        groceryManager,
        filename
    );
}

bool DataManager::saveData(
    const GroceryManager& manager,
    const std::string& filename
) const
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    json document;

    document["items"] = json::array();
    document["shoppingLists"] = json::array();
    document["budgets"] = json::array();
    document["users"] = json::array();
    document["purchases"] = json::array();

    for (const GroceryItem& item : manager.getItems())
    {
        document["items"].push_back(
            groceryItemToJson(item)
        );
    }

    for (
        const ShoppingList& shoppingList :
        manager.getShoppingLists()
    )
    {
        document["shoppingLists"].push_back(
            shoppingListToJson(shoppingList)
        );
    }

    for (const Budget& budget : manager.getBudgets())
    {
        document["budgets"].push_back(
            budgetToJson(budget)
        );
    }

    for (const UserProfile& user : manager.getUsers())
    {
        document["users"].push_back(
            userToJson(user)
        );
    }

    for (
        const PurchaseRecord& purchase :
        manager.getPurchases()
    )
    {
        document["purchases"].push_back(
            purchaseToJson(purchase)
        );
    }

    file << document.dump(4);

    return file.good();
}

bool DataManager::loadData(
    GroceryManager& manager,
    const std::string& filename
) const
{
    return loadJsonFile(manager, filename);
}
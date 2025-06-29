//
// Created by marce on 6/29/2025.
//

#ifndef INVENTORY_H
#define INVENTORY_H

#include <memory>
#include "InventoryBaseClass.h"
#include "../Consumables.h"

class Inventory
{
public:
    explicit Inventory(std::unique_ptr<Inventory_Interface> strategy);
    ~Inventory() = default;

    bool add_Item(std::shared_ptr<Consumables> item);
    bool remove_Item(const std::string& itemId);
    const std::vector<std::shared_ptr<Consumables>>& get_Items() const;
    bool has_Item(const std::string& itemId) const;
    size_t get_Item_Count() const;
    void clear();
    void use_Item(const std::string& itemId);


private:
    std::unique_ptr<Inventory_Interface> strategy_;

};

#endif //INVENTORY_H

//
// Created by marce on 6/28/2025.
//

#ifndef INVENTORYBASECLASS_H
#define INVENTORYBASECLASS_H
#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../Collidable.h"

class Collidable;

class Inventory_Interface {
public:
    virtual ~Inventory_Interface() = default;

    /// Add an item to the inventory.
    /// Returns true if the item was added successfully.
    virtual bool add_Item(std::shared_ptr<Collidable> item) = 0;

    /// Removes an item by ID or reference.
    /// Returns true if the item was found and removed.
    virtual bool remove_Item(const std::string& itemId) = 0;

    /// Returns a list of all items currently held.
    virtual const std::vector<std::shared_ptr<Collidable>>& get_Items() const = 0;

    /// Check if the inventory contains an item by its ID.
    virtual bool has_Item(const std::string& itemId) const = 0;

    /// Get the total number of items in the inventory.
    virtual size_t get_Item_Count() const = 0;

    /// Remove all items from the inventory.
    virtual void clear() = 0;
};

#endif //INVENTORYBASECLASS_H

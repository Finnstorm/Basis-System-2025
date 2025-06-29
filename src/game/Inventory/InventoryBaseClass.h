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

/*
 * If you want to implement an Inventory for your team, simply create a class inheriting from Inventory_Interface
 * then define the logic for all the functions within your TeamXInventory.cpp (or whatever you name it)
 * The Player constructor then receives a "std::unique_ptr<Inventory_Interface> inventory_Strategy)"
 * all concrete consumables need to define the logic for use() and get_ID()
 */

class Consumables;

class Inventory_Interface {
public:
    virtual ~Inventory_Interface() = default;

    /// Add an item to the inventory.
    /// Returns true if the item was added successfully.
    virtual bool add_Item(std::shared_ptr<Consumables> item) = 0;

    /// Removes an item by ID or reference.
    /// Returns true if the item was found and removed.
    virtual bool remove_Item(const std::string& itemId) = 0;

    /// Returns a list of all items currently held.
    virtual const std::vector<std::shared_ptr<Consumables>>& get_Items() const = 0;

    /// Check if the inventory contains an item by its ID.
    virtual bool has_Item(const std::string& itemId) const = 0;

    /// Get the total number of items in the inventory.
    virtual size_t get_Item_Count() const = 0;

    /// Remove all items from the inventory.
    virtual void clear() = 0;

    ///use the Item - define inside concrete Inventory implementation
    virtual void use_Item(const std::string& itemId) = 0;
};

#endif //INVENTORYBASECLASS_H

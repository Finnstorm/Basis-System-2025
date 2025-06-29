//
// Created by marce on 6/29/2025.
//

#include "Inventory.h"

Inventory::Inventory(std::unique_ptr<Inventory_Interface> strategy)
    : strategy_(std::move(strategy)) {}

bool Inventory::add_Item(std::shared_ptr<Consumables> item) {
    return strategy_->add_Item(std::move(item));
}

bool Inventory::remove_Item(const std::string& itemId) {
    return strategy_->remove_Item(itemId);
}

bool Inventory::has_Item(const std::string& itemId) const {
    return strategy_->has_Item(itemId);
}

size_t Inventory::get_Item_Count() const {
    return strategy_->get_Item_Count();
}

void Inventory::clear() {
    strategy_->clear();
}

void Inventory::use_Item(const std::string& itemId) {
    strategy_->use_Item(itemId);
}

const std::vector<std::shared_ptr<Consumables>>& Inventory::get_Items() const {
    return strategy_->get_Items();
}
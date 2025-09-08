//
// Created by $Will on 27.08.2025.
//

#ifndef FAIRYITEM_H
#define FAIRYITEM_H
#include "ItemBase.h"

class FairyItem : public ItemBase
{
public:
    FairyItem(Vector2 position, int current_level);
    void Activate(Player_Base_Class* player) override {}
};


#endif //FAIRYITEM_H

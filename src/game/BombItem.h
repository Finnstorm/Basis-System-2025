//
// Created by $Will on 27.08.2025.
//

#ifndef BOMBITEM_H
#define BOMBITEM_H

#include "ItemBase.h"

class BombItem : public ItemBase
{
public:
    BombItem(Vector2 position);
    void Activate(Player_Base_Class* player) override;
};



#endif //BOMBITEM_H

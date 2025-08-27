//
// Created by $Will on 27.08.2025.
//

#ifndef HEALTHPOTION_H
#define HEALTHPOTION_H

#include "ItemBase.h"

class HealthPotion : public ItemBase
{
public:
    HealthPotion(Vector2 position);
    void Activate(Player_Base_Class* player) override;
};



#endif //HEALTHPOTION_H

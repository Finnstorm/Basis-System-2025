//
// Created by $Will on 04.09.2025.
//

#ifndef KNIGHTENEMY_H
#define KNIGHTENEMY_H

#include "MeleeEnemy.h"

namespace enemy
{
    class Knight_Enemy : public Melee_Enemy
    {
    public:
        Knight_Enemy(Vector2 start_position);
        ~Knight_Enemy() override = default;
    };
}
#endif //KNIGHTENEMY_H

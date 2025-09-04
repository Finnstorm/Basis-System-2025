//
// Created by $Will on 04.09.2025.
//

#ifndef DEMONKNIGHTENEMY_H
#define DEMONKNIGHTENEMY_H

#include "MeleeEnemy.h"

namespace enemy
{
    class Demon_Knight_Enemy : public Melee_Enemy
    {
    public:
        Demon_Knight_Enemy(Vector2 start_position);
        ~Demon_Knight_Enemy() override = default;
    };
}
#endif //DEMONKNIGHTENEMY_H

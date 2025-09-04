//
// Created by $Will on 04.09.2025.
//

#ifndef PEASANTENEMY_H
#define PEASANTENEMY_H
#include "MeleeEnemy.h"
namespace enemy
{
    class Peasant_Enemy : public Melee_Enemy
    {
    public:
        Peasant_Enemy(Vector2 start_position);
        ~Peasant_Enemy() override = default;
    };
}



#endif //PEASANTENEMY_H

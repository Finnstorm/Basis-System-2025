//
// Created by Manza on 6/22/2025.
//

#pragma once
#include "EnemyBaseClass.h"
#include "../core/Animations.h"
#include "../core/RepeatAnimation.h"
#include <map>
#include "PlayerBaseClass.h"

enum EnemyState { E_IDLE, E_WALKING, E_ATTACKING };

namespace enemy {
    class Melee_Enemy : public Enemy_Base_Class {
    private:
        float attack_range;
        EnemyState currentState = E_WALKING;
        Facing_Direction facing_Direction = LEFT;

        std::map<Facing_Direction, RepeatAnimation> walk_animations;
        std::map<Facing_Direction, Animations> attack_animations;

    public:
        Melee_Enemy(Vector2 start_position);
        ~Melee_Enemy() override = default;

        void Tick(float delta_time, float target_Position_X, float target_Position_Y);
        void Draw() override;
        void Melee_Attack() override;
    };
}
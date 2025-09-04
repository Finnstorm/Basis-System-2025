//
// Created by $Will on 04.09.2025.
//

#include "DemonKnightEnemy.h"
#include "../Config.h.in"

namespace enemy
{
    Demon_Knight_Enemy::Demon_Knight_Enemy(Vector2 start_position)
    : Melee_Enemy(start_position, "Demonenritter",
              game::Config::melee_enemy_3_health, game::Config::melee_enemy_3_movement_speed,
              game::Config::melee_enemy_3_damage, game::Config::melee_enemy_3_score_value,
              game::Config::melee_enemy_3_souls_value, game::Config::melee_enemy_3_attack_cooldown,
              game::Config::melee_enemy_3_hitbox, game::Config::kMeleeEnemy3WalkLeftAnim,
              game::Config::kMeleeEnemy3WalkRightAnim, game::Config::kMeleeEnemy3AttackLeftAnim,
              game::Config::kMeleeEnemy3AttackRightAnim, game::Config::melee_enemy_3_walk_anim_size,
              game::Config::melee_enemy_3_walk_frame_count, game::Config::melee_enemy_3_walk_anim_speed,
              game::Config::melee_enemy_3_attack_anim_size, game::Config::melee_enemy_3_attack_frame_count,
              game::Config::melee_enemy_3_attack_anim_speed)
    {}
}
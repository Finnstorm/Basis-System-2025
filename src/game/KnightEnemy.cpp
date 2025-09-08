//
// Created by $Will on 04.09.2025.
//

#include "KnightEnemy.h"
#include "../Config.h.in"

namespace enemy
{
    Knight_Enemy::Knight_Enemy(Vector2 start_position)
    : Melee_Enemy(start_position, "Ritter",
              game::Config::melee_enemy_2_health, game::Config::melee_enemy_2_movement_speed,
              game::Config::melee_enemy_2_damage, game::Config::melee_enemy_2_score_value,
              game::Config::melee_enemy_2_souls_value, game::Config::melee_enemy_2_attack_cooldown,
              game::Config::melee_enemy_2_hitbox, game::Config::kMeleeEnemy2WalkLeftAnim,
              game::Config::kMeleeEnemy2WalkRightAnim, game::Config::kMeleeEnemy2AttackLeftAnim,
              game::Config::kMeleeEnemy2AttackRightAnim, game::Config::melee_enemy_2_walk_anim_size,
              game::Config::melee_enemy_2_walk_frame_count, game::Config::melee_enemy_2_walk_anim_speed,
              game::Config::melee_enemy_2_attack_anim_size, game::Config::melee_enemy_2_attack_frame_count,
              game::Config::melee_enemy_2_attack_anim_speed)
    {}
}
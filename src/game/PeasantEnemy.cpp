//
// Created by $Will on 04.09.2025.
//

#include "PeasantEnemy.h"
#include "../Config.h.in"

namespace enemy
{
    Peasant_Enemy::Peasant_Enemy(Vector2 start_position)
    : Melee_Enemy(start_position, "Bauer",
              game::Config::melee_enemy_1_health, game::Config::melee_enemy_1_movement_speed,
              game::Config::melee_enemy_1_damage, game::Config::melee_enemy_1_score_value,
              game::Config::melee_enemy_1_souls_value, game::Config::melee_enemy_1_attack_cooldown,
              game::Config::melee_enemy_1_hitbox, game::Config::kMeleeEnemy1WalkLeftAnim,
              game::Config::kMeleeEnemy1WalkRightAnim, game::Config::kMeleeEnemy1AttackLeftAnim,
              game::Config::kMeleeEnemy1AttackRightAnim, game::Config::melee_enemy_1_walk_anim_size,
              game::Config::melee_enemy_1_walk_frame_count, game::Config::melee_enemy_1_walk_anim_speed,
              game::Config::melee_enemy_1_attack_anim_size, game::Config::melee_enemy_1_attack_frame_count,
              game::Config::melee_enemy_1_attack_anim_speed)
    {}
}

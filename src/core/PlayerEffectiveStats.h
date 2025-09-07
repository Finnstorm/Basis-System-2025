#pragma once
#include <cmath>

#include "../config.h.in"
#include "Store.h"

struct PlayerEffectiveStats {
    int   max_health;
    float movement_speed;
    float meele_attack_cooldown;
    float ranged_attack_cooldown;
    float DMGxMult;
    int   meleeDamage;
    int   rangedDamage;

};

inline PlayerEffectiveStats BuildEffectiveStats(const game::core::UpgradeState& up) {
    PlayerEffectiveStats stats;
    // Bases from config (do NOT modify these directly)
    stats.max_health = game::Config::player_Class_One_Max_Health;
    stats.movement_speed = game::Config::player_Class_One_Movement_Speed; // apply later
    stats.ranged_attack_cooldown = game::Config::player_Melee_Attack_Cooldown;
    stats.meele_attack_cooldown = game::Config::player_Melee_Attack_Cooldown;
    stats.meleeDamage  = game::Config::player_Melee_Damage_Value;
    stats.rangedDamage = game::Config::player_Ranged_Damage_Value;
    stats.DMGxMult = game::Config::player_Class_One_Damage_Multiplier;

    // ---- Per-level rules (tune as needed) ----
    constexpr int   kHPPerLevel          = 10;        // +10 HP per level
    constexpr float kSpeedPerLevel       = 0.05f;     // +5% movespeed / level
    constexpr float kDmgMultPerLevel   = 0.05f;     // +5% all dmg / level
    constexpr int   kMeleePerLevel   = 10;        // +10 melee per level (additive)
    constexpr int   kRangedPerLevel  = 10;        // +10 ranged per level (additive)
    constexpr float kAtkSpeedPerLevel    = 0.08f;     // -8% cooldown / level (multiplicative)
    constexpr float kMinCooldownSeconds  = 0.15f;     // never go below this



    // ---- Apply upgrades ----
    // Health (additive)
    stats.max_health += kHPPerLevel * up.maxhealth_level;

    // Movement speed (multiplicative)
    stats.movement_speed *= (1.0f + kSpeedPerLevel * up.speed_level);

    // meele Attack cooldown (multiplicative reduction, then clamp)
    const float atk_cd_factor = std::max(0.0f, 1.0f - kAtkSpeedPerLevel * up.atkSpeed_level);
    stats.meele_attack_cooldown *= atk_cd_factor;
    if (stats.meele_attack_cooldown < kMinCooldownSeconds) stats.meele_attack_cooldown = kMinCooldownSeconds;

    // ranged Attack cooldown (multiplicative reduction, then clamp)
    const float rng_cd_factor = std::max(0.0f, 1.0f - kAtkSpeedPerLevel * up.atkSpeed_level);
    stats.ranged_attack_cooldown *= atk_cd_factor;
    if (stats.ranged_attack_cooldown < kMinCooldownSeconds) stats.ranged_attack_cooldown = kMinCooldownSeconds;

    //Meele and Ranged Damage
    stats.meleeDamage  += kMeleePerLevel  * up.meleeDMG_level;
    stats.rangedDamage += kRangedPerLevel * up.rangedDMG_level;

    //Global Damage Mult
    const float dmg_global_mult = (1.0f + kDmgMultPerLevel * up.DMGxmult_level);
    stats.meleeDamage  = static_cast<int>(std::round(stats.meleeDamage  * dmg_global_mult));
    stats.rangedDamage = static_cast<int>(std::round(stats.rangedDamage * dmg_global_mult));

    stats.DMGxMult = dmg_global_mult * game::Config::player_Class_One_Damage_Multiplier;

    return stats;
}

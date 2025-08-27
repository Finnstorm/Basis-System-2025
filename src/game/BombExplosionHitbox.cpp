//
// Created by $Will on 27.08.2025.
//

#include "BombExplosionHitbox.h"
#include "../game/CollisionResponse.h"
#include "../Config.h.in"
#include <raylib.h>

BombExplosionHitbox::BombExplosionHitbox(Rectangle rect, int dmg)
    : damage(dmg),
      lifetime(game::Config::bomb_Explosion_Visual_Lifetime),
      damage_active_timer(game::Config::bomb_Explosion_Damage_Lifetime),
      animation(game::Config::bomb_Explosion_Tile_Size, game::Config::kBombExplosionAnim,
                game::Config::bomb_Explosion_Frame_Count, game::Config::bomb_Explosion_Frame_Count, game::Config::bomb_Explosion_Anim_Speed)
{
    this->hitbox = rect;
}

void BombExplosionHitbox::Tick(float delta_time)
{
    lifetime -= delta_time;
    if (damage_active_timer > 0)
    {
        damage_active_timer -= delta_time;
    }

    if (lifetime <= 0.0f)
    {
        Mark_For_Destruction();
    }
    animation.Update_Frame(delta_time);
}

void BombExplosionHitbox::On_Collision(Collidable* other)
{
    // Schaden wird nur verursacht, solange der Timer läuft
    if (damage_active_timer > 0 && other->Get_Collision_Type() == Collision_Type::ENEMY)
    {
        CollisionResponse::Apply_Damage(other, damage);
    }
}

void BombExplosionHitbox::Draw()
{
    Texture2D sheet = animation.GetSpritesheet();
    Rectangle source_rec = animation.GetCurrentFrameRec();

    // Manuelle Implementierung der Kachel-Funktion
    if (sheet.id <= 0 || source_rec.width == 0 || source_rec.height == 0) return;

    for (float y = hitbox.y; y < hitbox.y + hitbox.height; y += source_rec.height)
    {
        for (float x = hitbox.x; x < hitbox.x + hitbox.width; x += source_rec.width)
        {
            DrawTextureRec(sheet, source_rec, {x, y}, WHITE);
        }
    }

    if (game::Config::visualize_Attack_Hitboxes)
    {
        DrawRectangleLinesEx(this->hitbox, 1.0f, (damage_active_timer > 0) ? RED : ORANGE);
    }
}
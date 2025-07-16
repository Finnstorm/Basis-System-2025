#include "PlayerProjectile.h"
#include "../config.h.in"
#include "CollisionResponse.h"
#include "raymath.h"

namespace game
{
    Player_Projectile::Player_Projectile(Vector2 start_position, Vector2 direction, float projectile_speed, int final_damage)
        : is_active(true), damage(final_damage)
    {
        this->sprite = LoadTexture(game::Config::kProjectileSprite);

        this->velocity = Vector2Scale(direction, projectile_speed);

        this->rotation = atan2(direction.y, direction.x) * RAD2DEG;

        this->hitbox = {
            start_position.x - game::Config::projectile_Hitbox_Size.x / 2.0f,
            start_position.y - game::Config::projectile_Hitbox_Size.y / 2.0f,
            game::Config::projectile_Hitbox_Size.x,
            game::Config::projectile_Hitbox_Size.y
        };
    }

    Player_Projectile::~Player_Projectile() {
        UnloadTexture(this->sprite);
    }

    void Player_Projectile::Tick(float delta_time) {
        if (!is_active) return;
        hitbox.x += velocity.x * delta_time;
        hitbox.y += velocity.y * delta_time;
    }

    void Player_Projectile::Draw()
    {
        if (!is_active) return;

        Rectangle sourceRec = { 0.0f, 0.0f, (float)this->sprite.width, (float)this->sprite.height };
        Rectangle destRec = { hitbox.x + hitbox.width/2, hitbox.y + hitbox.height/2, (float)this->sprite.width, (float)this->sprite.height };
        Vector2 origin = { (float)this->sprite.width / 2, (float)this->sprite.height / 2 };

        destRec.x = roundf(destRec.x);
        destRec.y = roundf(destRec.y);

        DrawTexturePro(this->sprite, sourceRec, destRec, origin, this->rotation, WHITE);
    }

    Collision_Type Player_Projectile::Get_Collision_Type() const { return Collision_Type::PLAYER_PROJECTILE; }

    void Player_Projectile::On_Collision(Collidable* other) {
        Collision_Type other_type = other->Get_Collision_Type();
        if (other_type == Collision_Type::ENEMY) {
            CollisionResponse::Apply_Damage(other, this->damage);
            this->Mark_For_Destruction();
        } else if (other_type == Collision_Type::WALL || other_type == Collision_Type::ENEMY_SPAWNER) {
            this->Mark_For_Destruction();
        }
    }
}

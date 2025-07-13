#include "PlayerProjectile.h"
#include "../config.h.in"
#include "CollisionResponse.h"
#include "raymath.h"

namespace game {
    // Der Konstruktor bleibt fast gleich, initialisiert aber nur noch die hitbox
    Player_Projectile::Player_Projectile(Vector2 start_position, Vector2 direction, float projectile_speed, int final_damage, Facing_Direction facing_dir)
        : is_active(true), damage(final_damage), animation({0,0}, nullptr, 0, 0, 0)
    {
        this->velocity = Vector2Scale(direction, projectile_speed);
        this->hitbox = {
            start_position.x - game::Config::projectile_Hitbox_Size.x / 2.0f,
            start_position.y - game::Config::projectile_Hitbox_Size.y / 2.0f,
            game::Config::projectile_Hitbox_Size.x,
            game::Config::projectile_Hitbox_Size.y
        };

        const char* anim_path = nullptr;
        switch (facing_dir) {
            case UP:         anim_path = game::Config::kProjectileAnimUp; break;
            case DOWN:       anim_path = game::Config::kProjectileAnimDown; break;
            case LEFT:       anim_path = game::Config::kProjectileAnimLeft; break;
            case RIGHT:      anim_path = game::Config::kProjectileAnimRight; break;
            case UP_LEFT:    anim_path = game::Config::kProjectileAnimUpLeft; break;
            case UP_RIGHT:   anim_path = game::Config::kProjectileAnimUpRight; break;
            case DOWN_LEFT:  anim_path = game::Config::kProjectileAnimDownLeft; break;
            case DOWN_RIGHT: anim_path = game::Config::kProjectileAnimDownRight; break;
        }

        // Initialisiere das Animationsobjekt mit dem korrekten Sprite
        if (anim_path) {
            this->animation = RepeatAnimation(
                game::Config::projectile_Anim_Size,
                anim_path,
                game::Config::projectile_Anim_Frame_Count,
                game::Config::projectile_Anim_Frame_Count,
                game::Config::projectile_Anim_Speed
            );
        }
        printf("--- Projektil Erzeugt ---\n");
        printf("Richtung: x=%.2f, y=%.2f\n", direction.x, direction.y);
        printf("Geschwindigkeit: %.2f\n", projectile_speed);
        printf("Finale Velocity: x=%.2f, y=%.2f\n", this->velocity.x, this->velocity.y);
        printf("------------------------\n");
    }

    Player_Projectile::~Player_Projectile() {}

    void Player_Projectile::Tick(float delta_time) {
        printf("Projektil Tick: dt=%.4f, hitbox.x vor Bewegung=%.2f\n", delta_time, hitbox.x);
        if (!is_active) return;

        hitbox.x += velocity.x * delta_time;
        hitbox.y += velocity.y * delta_time;

        animation.Update_Frame(delta_time);
    }

    void Player_Projectile::Draw()
    {
        if (!is_active) return;

        // Berechne die Zeichenposition, um die Hitbox zu zentrieren
        Vector2 draw_pos;
        draw_pos.x = this->hitbox.x - (animation.size.x - this->hitbox.width) / 2.0f;
        draw_pos.y = this->hitbox.y - (animation.size.y - this->hitbox.height) / 2.0f;

        animation.Draw_Current_Frame(draw_pos);

        // Optional: Zeichne die Hitbox zum Debuggen
        DrawRectangleLinesEx(this->hitbox, 1, VIOLET);
    }

    Collision_Type Player_Projectile::Get_Collision_Type() const {
        return Collision_Type::PLAYER_PROJECTILE;
    }

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

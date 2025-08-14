//
// Created by $Will on 14.08.2025.
//

#include "PlayerMeleeHitbox.h"
#include "CollisionResponse.h"
namespace game
{
    Player_Melee_Hitbox::Player_Melee_Hitbox(Rectangle hitbox, int damage)
        : lifetime(game::Config::player_Melee_Hitbox_Lifetime), damage(damage) {
        this->hitbox = hitbox;
    }

    void Player_Melee_Hitbox::Tick(float delta_time)
    {
        lifetime -= delta_time;
        if (lifetime <= 0) {
            is_Marked_For_Destruction = true;
        }
    }
    void Player_Melee_Hitbox::On_Collision(Collidable* other)
    {
        // Wir prüfen nur auf Kollision mit Gegnern
        if (other->Get_Collision_Type() == Collision_Type::ENEMY) {
            bool already_hit = false;
            // Prüfen, ob wir diesen Gegner in diesem Angriff schon getroffen haben
            for (Collidable* target : hit_targets) {
                if (target == other) {
                    already_hit = true;
                    break;
                }
            }

            // Wenn nicht, Schaden anwenden und zur Liste hinzufügen
            if (!already_hit) {
                // Der entscheidende Aufruf an deine statische Helferfunktion
                CollisionResponse::Apply_Damage(other, this->damage);

                // Gegner zur Liste der getroffenen Ziele hinzufügen
                hit_targets.push_back(other);
            }
        }
    }
    void Player_Melee_Hitbox::Draw()
    {
        if (game::Config::visualize_Attack_Hitboxes)
        {
            DrawRectangleRec(this->hitbox, Fade(RED, 0.5f));
        }
    }
}
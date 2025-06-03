//
// Created by Kruse on 23/05/2025.
//

#include "PlayerInput.h"
#include "../config.h.in"
#include <cmath>
#include "store.h"


namespace game::core
{
    // Spieler Input
    Player::Player(Vector2 start_Position, float move_Speed)
    : current_position(start_Position), movement_speed(move_Speed)
    {
    //Steuerungstasten aus globaler Config

    key_Up = game::Config::key_Up;
    key_Down = game::Config::key_Down;
    key_Left = game::Config::key_Left;
    key_Right = game::Config::key_Right;

    key_Melee_Attack = game::Config::key_Melee_Attack;

    player_width = game::Config::player_Width;
    player_height = game::Config::player_Height;
    attack_duration = game::Config::player_Attack_Duration;
    attack_Cooldown = game::Config::player_Attack_Cooldown;
    }

    // Prüft Tasteneingabe, normalisiert bewegung, kontrolliert Angriffsdauer, setzt Spieler Richtung
    Vector2 Player::Update()
    {
    Vector2 movement_this_frame = {0.0f, 0.0f};
    Vector2 input_direction = {0.0f, 0.0f};

    float delta_time = 0.0f;
    if (game::core::Store::dtm != nullptr)
    {
        delta_time = game::core::Store::dtm->Get_Dt();
    }

    // Cooldown Timer aktualisieren
    if (current_attack_cooldown_timer > 0.0f) // current_attack_cooldown_timer ist Member
    {
        current_attack_cooldown_timer -= delta_time;
        if (current_attack_cooldown_timer < 0.0f)
        {
            current_attack_cooldown_timer = 0.0f;
        }
    }

    // Tasteneingabe für Bewegung prüfen
    if (IsKeyDown(key_Up))    input_direction.y -= 1.0f;
    if (IsKeyDown(key_Down))  input_direction.y += 1.0f;
    if (IsKeyDown(key_Left))  input_direction.x -= 1.0f;
    if (IsKeyDown(key_Right)) input_direction.x += 1.0f;

    // Bewegung berechnen, wenn Input vorhanden ist
    if (input_direction.x != 0.0f || input_direction.y != 0.0f)
    {
        float length = sqrtf(input_direction.x * input_direction.x + input_direction.y * input_direction.y);
        // Normalisierter Richtungsvektor (input_direction wird hier direkt für die Richtung verwendet)
        Vector2 normalized_direction = {input_direction.x / length, input_direction.y / length};

        // Berechne die Bewegung für diesen Frame
        movement_this_frame.x = normalized_direction.x * movement_speed * delta_time;
        movement_this_frame.y = normalized_direction.y * movement_speed * delta_time;
    }
    // Wenn kein Input, bleibt movement_this_frame {0.0f, 0.0f}

    // Nahkampfangriff starten
    if (IsKeyPressed(key_Melee_Attack) && !is_attacking && current_attack_cooldown_timer <= 0.0f)
    {
        is_attacking = true;
        attack_timer = attack_duration;
    }

    // Angriff aktualisieren
    if (is_attacking)
    {
        attack_timer -= delta_time;
        if (attack_timer <= 0.0f)
        {
            is_attacking = false;
            attack_timer = 0.0f;
            current_attack_cooldown_timer = attack_Cooldown; // attack_Cooldown ist Member
        }
    }

    // Richtung setzen basierend auf der reinen Eingabe (input_direction)
    if (input_direction.x < 0 && input_direction.y < 0) last_direction = LeftUp;
    else if (input_direction.x > 0 && input_direction.y < 0) last_direction = RightUp;
    else if (input_direction.x < 0 && input_direction.y > 0) last_direction = LeftDown;
    else if (input_direction.x > 0 && input_direction.y > 0) last_direction = RightDown;
    else if (input_direction.x < 0) last_direction = Left;
    else if (input_direction.x > 0) last_direction = Right;
    else if (input_direction.y < 0) last_direction = Up;
    else if (input_direction.y > 0) last_direction = Down;
    // Wenn keine Bewegungseingabe, bleibt last_direction unverändert.

    return movement_this_frame;
    }

    Vector2 Player::Get_Position() const
    {
        return this->current_position;
    }

    void Player::Set_Position(Vector2 new_position)
    {
        this->current_position = new_position;
    }

    bool Player::Is_Attacking() const
    {
        return this->is_attacking;
    }

    // Bestimmt die Nahkampfhittbox je nach Richtung
    Rectangle Player::GetAttackHitbox() const
    {
    if (!is_attacking) return { 0, 0, 0, 0 };

        const float hitbox_size = game::Config::player_Hitbox_Size;
        Vector2 hitbox_center_position = current_position;

        // Offset der Hitbox basierend auf der Richtung des Spielers
        // Dies sind Offsets relativ zur Spielerposition (current_position),
        // die als Zentrum der Hitbox interpretiert wird.
        // Die Werte hier (z.B. hitbox_size) könnten auch aus Config stammen.

    switch (last_direction)
    {
        case Up:        hitbox_center_position.y -= hitbox_size; break;
        case Down:      hitbox_center_position.y += hitbox_size; break;
        case Left:      hitbox_center_position.x -= hitbox_size; break;
        case Right:     hitbox_center_position.x += hitbox_size; break;
        case LeftUp:    hitbox_center_position.x -= hitbox_size; hitbox_center_position.y -= hitbox_size; break;
        case RightUp:   hitbox_center_position.x += hitbox_size; hitbox_center_position.y -= hitbox_size; break;
        case LeftDown:  hitbox_center_position.x -= hitbox_size; hitbox_center_position.y += hitbox_size; break;
        case RightDown: hitbox_center_position.x += hitbox_size; hitbox_center_position.y += hitbox_size; break;
    }

        // Erstellen eines Rechteckes für die Hitbox
    return Rectangle
    {
        hitbox_center_position.x - hitbox_size / 2, hitbox_center_position.y - hitbox_size / 2,
        hitbox_size, hitbox_size };
    }

    // Bestimmt Erscheinungsbild des Spielers
    void Player::Draw()
    {
        Rectangle player_rect =
        {
            current_position.x - player_width / 2.0f, // Obere linke Ecke X
            current_position.y - player_height / 2.0f, // Obere linke Ecke Y
            player_width,  // Breite
            player_height // Höhe
        };
        DrawRectangleRec(player_rect, BLUE);
    }
}
//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_PLAYERINPUT_H
#define RAYLIBSTARTER_PLAYERINPUT_H

#include "raylib.h"
#include "DeltaTimeMachine.h"

//Vieles hier ist Teamspezifisch
namespace game::core
{
    class Player
    {
    public:
        enum Direction
    {
        Up, Down, Left, Right, LeftDown, RightDown, LeftUp, RightUp
    };

    // Konstruktor initialisiert Spieler mit Start Position und Bewegungsgeschwindigkeit
    Player(Vector2 start_Position, float move_Speed);

    // Wird mit jedem Frame aufgerufen, um Position zu aktualisieren
    Direction Update();

    // Zeichnet den Spieler
    void Draw();

    // Gibt die aktuelle Spielerposition zurück
    Vector2 Get_Position() const;

    // Gibt an, ob gerade ein Angriff ausgeführt wird
    bool Is_Attacking() const;

    // Gibt die Hitbox dese aktuellen Angriffs zurück
    Rectangle GetAttackHitbox() const;

private:

    // Aktuelle Position des Spielers
    Vector2 current_position;

    // Bewegungsgeschwindigkeit
    float movement_speed;

    // Abmessung der Spielfigur
    float player_width;
    float player_height;

    // Tastenzuweisung für die Bewegung
    int key_Up;
    int key_Down;
    int key_Left;
    int key_Right;

    // Taste für den Nahkampfangriff
    int key_Melee_Attack;

    // Angriffssteuerung
    bool is_attacking=false;
    float attack_duration;
    float attack_timer = 0.0f;
    float attack_Cooldown;
    float current_attack_cooldown_timer = 0.0f;

    // Zuletzt registrierte Bewegungsrichtung
    Direction last_direction = Down;
    };
}

#endif //RAYLIBSTARTER_PLAYERINPUT_H

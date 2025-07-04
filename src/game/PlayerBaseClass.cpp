//
// Created by Kruse on 23/05/2025.
//

#include <iostream>
#include "PlayerBaseClass.h"
#include "CollisionResponse.h"
#include "Consumables.h"
#include "EnemyBaseClass.h"

// Konstruktor
Player_Base_Class::Player_Base_Class(int max_Health, float movement_Speed, int damage, Vector2 start_Position,
     Collision_Manager* manager)
    : player_Max_Health(max_Health), player_Health((float)max_Health), player_Movement_Speed(movement_Speed),
      player_Damage(damage),
      previous_Position(start_Position), manager_Ptr(manager), melee_Cooldown(0.0f), ranged_Cooldown(0.0f),
      inventory_Is_Full(false), facing_Direction(Facing_Direction::DOWN), is_Moving(false)
{
    hitbox={start_Position.x,start_Position.y,static_cast<float >(maintex.width),static_cast<float >(maintex.height)};
    // 2. Registriere Objekt beim Manager

}

void Player_Base_Class::Register_Player() {
    if (manager_Ptr)
    {
        manager_Ptr->Regist_Object(this);
    }
}
// Destruktor
Player_Base_Class::~Player_Base_Class()
{
    if (manager_Ptr)
    {
        manager_Ptr->Unregist_Object(this);
    }
}

// Phase 1 :: Player input Prüfung
void Player_Base_Class::Player_Input()
{
    if (IsKeyPressed(game::Config::key_Melee_Attack) && melee_Cooldown <= 0)
    {
        Melee_Attack();
    }

    if (IsKeyPressed(game::Config::key_Ranged_Attack) && ranged_Cooldown <= 0)
    {
        Ranged_Attack();
    }

    /*if (IsKeyPressed(game::Config::key_Use_Item) && inventory_Is_Full)
    {
        Use_Item();
    }*/
}

// Phase 2 :: Verwaltung für alles was das Objekt über eine gewisse Zeit machen soll
void Player_Base_Class::Tick(float delta_time)
{
    if (game::Config::enable_Health_Drain)
    {
        player_Health -= game::Config::player_Health_Drain_Rate * delta_time;
    }
    Update_Previous_Position();

	Vector2 move_Direction = {0.0f, 0.0f};
    if (IsKeyDown(game::Config::key_Up))    move_Direction.y -= 1.0f;
    if (IsKeyDown(game::Config::key_Down))  move_Direction.y += 1.0f;
    if (IsKeyDown(game::Config::key_Left))  move_Direction.x -= 1.0f;
    if (IsKeyDown(game::Config::key_Right)) move_Direction.x += 1.0f;

 	is_Moving = (move_Direction.x != 0.0f || move_Direction.y != 0.0f);
    if (is_Moving)
    {
        move_Direction = Vector2Normalize(move_Direction);
    }

    hitbox.x += floor(move_Direction.x * player_Movement_Speed * delta_time);
    hitbox.y += floor(move_Direction.y * player_Movement_Speed * delta_time);
    player_Pos.x=hitbox.x;
    player_Pos.y=hitbox.y;
    std::cout<< this->player_Pos.x<<"  "<< this->player_Pos.y<<"\n";

    Update_Facing_Direction();

    if (melee_Cooldown > 0) melee_Cooldown -= delta_time;
    if (ranged_Cooldown > 0) ranged_Cooldown -= delta_time;
}

// Phase 3 :: Kollisionsreaktion falls der Collisionmanager eine Kollision mit einem anderen Objekt feststellt
void Player_Base_Class::On_Collision(Collidable* other)
{
	Collision_Type other_Type = other->Get_Collision_Type();

    switch(other_Type)
    {
        case Collision_Type::WALL:
        case Collision_Type::ENEMY_SPAWNER:
        case Collision_Type::ENEMY:
        {
            if (this->is_Moving)
            {
                CollisionResponse::Resolve_Overlap(this, other);
            }
            break;
        }

        case Collision_Type::ENEMY_PROJECTILE:
        {
            /*if(auto* projectile = dynamic_cast<Enemy_Projectile*>(other))
            {
                this->Take_Damage(projectile->Get_Damage());
            }
            CollisionResponse::Mark_For_Destruction(other);*/
            break;
        }

        case Collision_Type::CONSUMABLE:
        {
            /* // Wir brauchen eine Basis-Klasse "Consumable", von der alle Items erben.
            if(auto* item = dynamic_cast<Consumables*>(other)) // Annahme: Es gibt eine Klasse Consumable
            {
                if(item->Is_Inventory_Item()) // Methode in Consumable, die den bool zurückgibt
                {
                    // Logik, um das Item dem Inventar hinzuzufügen
                    // inventory.Add(item);
                }
                else
                {
                    // Item wird sofort verwendet (z.B. Heilung)
                    item->Apply_Effect(this); // Jedes Item weiß selbst, was es tut
                }

                // In jedem Fall wird das Item aus der Welt entfernt
                CollisionResponse::Mark_For_Destruction(other);
            } */
            break;
        }

        default:
            break;
    }
}

// Draw Methode ist noch nicht klar, wie das mit der Visualisierung laufen wird
void Player_Base_Class::Draw()
{
    DrawRectangleLinesEx(hitbox,2,RED);
    DrawTexture(this->maintex, this->hitbox.x,hitbox.y,WHITE);
}

// Um die beiden Attack Methoden weiter auszuarbeiten, braucht es die passenden Klassen
void Player_Base_Class::Melee_Attack()
{
	melee_Cooldown = 0.0f;
}
void Player_Base_Class::Ranged_Attack()
{
	ranged_Cooldown = 0.0f;
}

// Funktion für die Tick Methode welche die aktuelle Position speichert, falls das Objekt zurück gesetzt werden soll
void Player_Base_Class::Update_Previous_Position()
{
    previous_Position.x = hitbox.x;
    previous_Position.y = hitbox.y;
}

// In PlayerBaseClass.cpp hinzufügen:
void Player_Base_Class::Set_Position(Vector2 position)
{
    this->hitbox.x = position.x;
    this->hitbox.y = position.y;
}
// Methode aus der Tick welche die aktuelle Blickrichtung zurück geben soll. Wird später fürs Zeichnen und für die
// Angriffe genutzt
void Player_Base_Class::Update_Facing_Direction()
{
    bool up = IsKeyDown(game::Config::key_Up);
    bool down = IsKeyDown(game::Config::key_Down);
    bool left = IsKeyDown(game::Config::key_Left);
    bool right = IsKeyDown(game::Config::key_Right);

    if ((up && down) || (left && right)) return;

    if (up && right) facing_Direction = Facing_Direction::UP_RIGHT;
    else if (up && left) facing_Direction = Facing_Direction::UP_LEFT;
    else if (down && right) facing_Direction = Facing_Direction::DOWN_RIGHT;
    else if (down && left) facing_Direction = Facing_Direction::DOWN_LEFT;
    else if (up) facing_Direction = Facing_Direction::UP;
    else if (down) facing_Direction = Facing_Direction::DOWN;
    else if (left) facing_Direction = Facing_Direction::LEFT;
    else if (right) facing_Direction = Facing_Direction::RIGHT;
}

// Getter für Player Hittbox und Collision Type


Collision_Type Player_Base_Class::Get_Collision_Type() const
{
    return Collision_Type::PLAYER;
}

Vector2 Player_Base_Class::Get_Player_Pos() {
    return this->player_Pos;
}

void Player_Base_Class::Take_Damage(int damage_amount)
{
    player_Health -= damage_amount;
}
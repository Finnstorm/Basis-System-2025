//
// Created by Kruse on 23/05/2025.
//

#include "EnemyBaseClass.h"
#include "CollisionManager.h"
#include "CollisionResponse.h"
#include "PlayerBaseClass.h"

namespace enemy
{
Enemy_Base_Class::Enemy_Base_Class(std::string name, int health, float movement_Speed, int damage, int value,
    const char* sprite_Path, Vector2 start_Position, int width, int height, Collision_Manager* manager_Ptr,
    float cooldown_Duration)
    : enemy_Name(name), enemy_Health(health), enemy_Movement_Speed(movement_Speed), enemy_Damage(damage),
    enemy_Value(value), attack_Cooldown_Duration(cooldown_Duration), attack_Cooldown_Timer(0.0f), is_Moving (false)
        {
            hitbox = {start_Position.x, start_Position.y, (float)width, (float)height};
            sprite = LoadTexture(sprite_Path);
        }

Enemy_Base_Class::~Enemy_Base_Class()
{
    UnloadTexture(sprite);
}

void Enemy_Base_Class::Take_Damage(int damage_amount)
{
    enemy_Health -= damage_amount;
}

void Enemy_Base_Class::Pathfinding()
{

}


//Core Methoden
void Enemy_Base_Class::Tick(float delta_time)
{
    if (attack_Cooldown_Timer > 0)
    {
        attack_Cooldown_Timer -= delta_time;
    }
    // Wenn der Pathfinding die Position veränder dann this->is_Moving = true;
}

void Enemy_Base_Class::On_Collision(Collidable* other)
{
        Collision_Type other_Type = other->Get_Collision_Type();

        switch(other_Type)
        {
            case Collision_Type::WALL:
            case Collision_Type::ENEMY_SPAWNER:
            case Collision_Type::PLAYER:
            {
                if (this->is_Moving)
                {
                    CollisionResponse::Resolve_Overlap(this, other);
                }

                if (other_Type == Collision_Type::PLAYER)
                {
                    // 2. Prüfen, ob der Angriff bereit ist.
                    if (attack_Cooldown_Timer <= 0)
                    {
                        if (auto* player = dynamic_cast<Player_Base_Class*>(other))
                        {
                            // 3. Schaden austeilen und Cooldown zurücksetzen.
                            player->Take_Damage(this->enemy_Damage);
                            this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;
                        }
                    }
                }
                break;
            }
            case Collision_Type::PLAYER_PROJECTILE:
            {
                /*if(auto* projectile = dynamic_cast<Player_Projectile*>(other))
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

void Enemy_Base_Class::Draw()
{

}
}

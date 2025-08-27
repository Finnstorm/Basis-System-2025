//
// Created by $Will on 27.08.2025.
//

#include "HealthPotion.h"
#include "PlayerBaseClass.h"
#include "../Config.h.in"

HealthPotion::HealthPotion(Vector2 position)
    // Wir rufen den Konstruktor der Basisklasse (ItemBase) auf und übergeben die spezifischen Werte für den Heiltrank
    : ItemBase(position, ItemType::HEALTH_POTION, game::Config::kHealthPotionAnim, true,
               game::Config::health_Potion_Anim_Size, game::Config::health_Potion_Frame_Count, game::Config::health_Potion_Anim_Speed)
{
    // Der Konstruktor ist leer, da die ganze Arbeit in der Initialisierungsliste erledigt wird.
}

void HealthPotion::Activate(Player_Base_Class* player)
{
    // Die Logik des Heiltranks:
    // 1. Heile den Spieler (Take_Damage mit negativem Wert)
    player->Take_Damage(-game::Config::health_Potion_Heal_Amount);

    // 2. Entferne und lösche das Item aus dem Inventar des Spielers
    player->RemoveHeldItem();
}
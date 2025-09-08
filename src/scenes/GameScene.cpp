#include "GameScene.h"
#include <memory>
#include <string>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include "Screen.h"
#include <Store.h>
#include "PauseScene.h"
#include "Renderer.h"
#include "SpriteAnimated.h"
#include <raymath.h>
#include "DeathScene.h"
#include "MenuScene.h"
#include "../game/PlayerClassOne.h"
#include "../core/CollisionManager.h"
#include "../game/MeleeEnemy.h"
#include "../game//TestoNeedle.h"
#include "../game/KeyItem.h"
#include "../game/FairyItem.h"
#include "../game/HealthPotion.h"
#include "../game/BombItem.h"
#include "../game/KnightEnemy.h"
#include "../game/DemonKnightEnemy.h"
#include "../game/PeasantEnemy.h"

using namespace std::string_literals;

game::scenes::GameScene::GameScene(int level_to_load) : Level_Nbr(level_to_load)
{
    enemy::Melee_Enemy::Load_All_Melee_Assets();

    if (!game::core::Store::player_state)
    {
        game::core::Store::player_state = std::make_unique<game::core::PlayerState>(game::Config::player_Spawn_Position);
    }
    else
    {
        game::core::Store::player_state->player.Set_Position(game::Config::player_Spawn_Position);
        game::core::Store::player_state->player.Heal_To_Full();
        game::core::Store::player_state->player.ReapplyUpgrades();
        game::core::Store::player_state->player.Reset_For_New_Level();
    }

    this->player_ptr = &game::core::Store::player_state->player;
    dtm.Start();
    objectManager.AddObject(this->player_ptr);
    cam = std::make_shared<Cam>(*this->player_ptr);

    hud= HUD(player_ptr);

    screen.Load_Game_Objects(objectManager);

    this->player_ptr->object_manager_ptr = &objectManager;
    p_cm = std::make_unique<Collision_Manager>(wb, objectManager.managed_objects);

    enemySpawner = std::make_unique<EnemySpawner>(objectManager, cam);
    enemySpawner->Register_Enemy_Type("Bauer", [](Vector2 pos) -> enemy::Enemy_Base_Class*
    {
        return new enemy::Peasant_Enemy(pos);
    });
    enemySpawner->Register_Enemy_Type("Ritter", [](Vector2 pos) -> enemy::Enemy_Base_Class*
    {
        return new enemy::Knight_Enemy(pos);
    });
    enemySpawner->Register_Enemy_Type("Demonenritter", [](Vector2 pos) -> enemy::Enemy_Base_Class*
    {
        return new enemy::Demon_Knight_Enemy(pos);
    });

    this->current_level = level_to_load;
    this->current_wave = 0;
    this->wave_timer = game::Config::kFirstWave;

    score_timer = 0.0f;

    objectManager.AddObject(new TestoNeedle(game::Config::initial_Testo_Needle_Position, false));
    objectManager.AddObject(new KeyItem(game::Config::initial_Key_Position));
}
game::scenes::GameScene::~GameScene()
{
    enemy::Melee_Enemy::Unload_All_Melee_Assets();
}

void game::scenes::GameScene::Update()
{
    if (player_ptr->Is_Dead())
    {
        bool player_has_fairy = player_ptr->HasFairy();
        auto deathScene = std::make_shared<DeathScene>(game::core::Store::player_state->score,
        game::core::Store::player_state->souls, player_has_fairy, this->current_level);
        game::core::Store::stage->SwitchToNewScene("DeathScene", deathScene);

        return;
    }
    score_timer += dtm.Get_Dt() / 10.0f;
    if (score_timer >= game::Config::kScore_Time_Interval)
    {
        game::core::Store::player_state->score++;
        score_timer -= game::Config::kScore_Time_Interval;
    }

    wave_timer -= dtm.Get_Dt();
    if (wave_timer <= 0.0f)
    {
        current_wave++;
        game::core::Store::player_state->score += game::Config::kScore_Per_Wave;
        enemySpawner->Start_New_Wave(current_wave, current_level);
        wave_timer = game::Config::kWaveInterval;
    }

    enemySpawner->Update(dtm.Get_Dt());
    player_ptr->Player_Input();

    std::vector<enemy::Enemy_Base_Class*> all_enemies;
    for (auto* object : objectManager.managed_objects)
    {
        if (auto* enemy = dynamic_cast<enemy::Enemy_Base_Class*>(object))
        {
            all_enemies.push_back(enemy);
        }
    }

    Vector2 player_center = player_ptr->Get_Player_Center();

    for (auto* object : objectManager.managed_objects)
    {
        object->Tick(dtm.Get_Dt());

        if (auto* enemy = dynamic_cast<enemy::Enemy_Base_Class*>(object))
        {
            enemy->Tick_AI(dtm.Get_Dt(), player_center, all_enemies);

            if (auto* melee_enemy = dynamic_cast<enemy::Melee_Enemy*>(enemy))
            {
                melee_enemy->Tick_Melee(dtm.Get_Dt(), player_center);
            }
        }
    }
    if (!fairy_has_spawned && !player_ptr->HasFairy())
    {
        objectManager.AddObjectDeferred(new FairyItem(game::Config::fairy_Spawn_Position, this->current_level));
        fairy_has_spawned = true;
    }

    if (!player_ptr->HasItem())
    {
        for (auto* object : objectManager.managed_objects)
        {
            if (auto* item = dynamic_cast<ItemBase*>(object))
            {
                if (CheckCollisionPointRec(player_center, item->GetAttractionHitbox()))
                {
                    Vector2 direction = Vector2Subtract(player_center, item->Get_Position());
                    direction = Vector2Normalize(direction);
                    Vector2 new_pos = Vector2Add(item->Get_Position(), Vector2Scale(direction, game::Config::item_Attraction_Speed * dtm.Get_Dt()));
                    item->Set_Position(new_pos);
                }
            }
        }
    }
    p_cm->Check_Collisions();
    cam->Cam_Movement(dtm.Get_Dt(), screen.Get_Map_Dimensions());
    std::vector<Vector2> dead_enemy_positions;
objectManager.Cleanup_Objects([this, &dead_enemy_positions](Collidable* cleaned_obj)
{
    if (cleaned_obj->Get_Collision_Type() == Collision_Type::ENEMY)
    {
        auto* enemy = static_cast<enemy::Enemy_Base_Class*>(cleaned_obj);
        game::core::Store::player_state->score += enemy->Get_Score_Value();
        game::core::Store::player_state->souls += enemy->Get_Souls_Value();

        dead_enemy_positions.push_back(enemy->Get_Position());
    }
});

    int potions_to_spawn = 0;
    int bombs_to_spawn = 0;
    int needles_to_spawn = 0;

for (const auto& pos : dead_enemy_positions)
{
    if (GetRandomValue(1, 100) <= game::Config::enemy_Item_Drop_Chance_Percent)
    {
        std::vector<std::pair<ItemType, int>> weighted_list;
        int total_weight = 0;

        if (CountItemsOfType(ItemType::HEALTH_POTION, objectManager, *player_ptr) + potions_to_spawn < game::Config::health_Potion_Max_On_Map) {
            weighted_list.push_back({ItemType::HEALTH_POTION, game::Config::item_Drop_Weight_Heal});
            total_weight += game::Config::item_Drop_Weight_Heal;
        }
        if (CountItemsOfType(ItemType::BOMB, objectManager, *player_ptr) + bombs_to_spawn < game::Config::bomb_Max_On_Map) {
            weighted_list.push_back({ItemType::BOMB, game::Config::item_Drop_Weight_Bomb});
            total_weight += game::Config::item_Drop_Weight_Bomb;
        }
        if (CountItemsOfType(ItemType::TESTO_NEEDLE, objectManager, *player_ptr) + needles_to_spawn < game::Config::testo_Needle_Max_On_Map) {
            weighted_list.push_back({ItemType::TESTO_NEEDLE, game::Config::item_Drop_Weight_TestoNeedle});
            total_weight += game::Config::item_Drop_Weight_TestoNeedle;
        }

        if (total_weight > 0)
        {
            int roll = GetRandomValue(1, total_weight);
            for (const auto& pair : weighted_list)
            {
                roll -= pair.second;
                if (roll <= 0)
                {
                    ItemBase* spawned_item = nullptr;
                    switch (pair.first)
                    {
                        case ItemType::HEALTH_POTION:
                            spawned_item = new HealthPotion(pos);
                            potions_to_spawn++;
                            break;
                        case ItemType::BOMB:
                            spawned_item = new BombItem(pos);
                            bombs_to_spawn++;
                            break;
                        case ItemType::TESTO_NEEDLE:
                            spawned_item = new TestoNeedle(pos);
                            needles_to_spawn++;
                            break;
                    }
                    if (spawned_item) {
                        objectManager.AddObjectDeferred(spawned_item);
                    }
                    break;
                }
            }
        }
    }
}

    hud.HUD_update();
objectManager.ProcessAdditions();
    dtm.Update();
}

void game::scenes::GameScene::Draw()
{
    BeginMode2D(this->cam->cam);
    screen.Draw_Level(this->cam, false);
    std::sort(objectManager.managed_objects.begin(), objectManager.managed_objects.end(),
        [](const Collidable* a, const Collidable* b) {
            return a->GetYSortPosition() < b->GetYSortPosition();
        });
    for(auto* obj : objectManager.managed_objects)
    {
        obj->Draw();
    }
    screen.Draw_Level(this->cam, true);
    EndMode2D();
    hud.HUD_draw();
}

int game::scenes::GameScene::CountItemsOfType(ItemType type, const Object_Manager& objectManager, const Player_Class_One& player)
{
    int count = 0;
    for (const auto* obj : objectManager.managed_objects)
    {
        if (const auto* item = dynamic_cast<const ItemBase*>(obj))
        {
            if (item->GetType() == type)
            {
                count++;
            }
        }
    }
    if (player.HasItem() && player.GetHeldItem()->GetType() == type)
    {
        count++;
    }
    return count;
}
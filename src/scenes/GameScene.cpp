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
#include "MenuScene.h"
#include "../game/PlayerClassOne.h"
#include "../core/CollisionManager.h"
#include "../game/MeleeEnemy.h"
#include "../game//TestoNeedle.h"
#include "../game/KeyItem.h"
#include "../game/FairyItem.h"
#include "../game/HealthPotion.h"
#include "../game/BombItem.h"

using namespace std::string_literals;

game::scenes::GameScene::GameScene()
{
    enemy::Melee_Enemy::Load_Assets();
    dtm.Start();
    objectManager.AddObject(&mp);
    cam = std::make_shared<Cam>(mp);

    screen.Load_Game_Objects(objectManager);

    mp.object_manager_ptr = &objectManager;
    p_cm = std::make_unique<Collision_Manager>(wb, objectManager.managed_objects);

    enemySpawner = std::make_unique<EnemySpawner>(objectManager, cam);
    enemySpawner->Register_Enemy_Type("Bauer", [](Vector2 pos) -> enemy::Enemy_Base_Class* {
        return new enemy::Melee_Enemy(pos);
    });

    this->current_level = 1;
    this->current_wave = 0;
    this->wave_timer = game::Config::kFirstWave;

    int score = 0;
    int souls = 0;
    float score_timer = 0.0f;

    objectManager.AddObject(new TestoNeedle(game::Config::initial_Testo_Needle_Position));
    objectManager.AddObject(new KeyItem(game::Config::initial_Key_Position));
}
game::scenes::GameScene::~GameScene()
{
}

void game::scenes::GameScene::Update()
{
    if (mp.Is_Dead())
    {
        game::core::Store::last_score = this->score;
        auto newMenuScene = std::make_shared<game::scenes::MenuScene>();
        game::core::Store::stage->SwitchToNewScene("MenuScene", newMenuScene);
        return;
    }
    score_timer += dtm.Get_Dt() / 10.0f;
    if (score_timer >= game::Config::kScore_Time_Interval)
    {
        score++;
        score_timer -= game::Config::kScore_Time_Interval;
    }

    wave_timer -= dtm.Get_Dt();
    if (wave_timer <= 0.0f)
    {
        current_wave++;
        score += game::Config::kScore_Per_Wave;
        enemySpawner->Start_New_Wave(current_wave, current_level);
        wave_timer = game::Config::kWaveInterval;
    }

    enemySpawner->Update(dtm.Get_Dt());
    mp.Player_Input();
    enemySpawner->Update(dtm.Get_Dt());
    mp.Player_Input();

    std::vector<enemy::Enemy_Base_Class*> all_enemies;
    for (auto* object : objectManager.managed_objects)
    {
        if (auto* enemy = dynamic_cast<enemy::Enemy_Base_Class*>(object))
        {
            all_enemies.push_back(enemy);
        }
    }

    Vector2 player_center = mp.Get_Player_Center();

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
    if (!fairy_has_spawned && !mp.HasFairy())
    {
        objectManager.AddObjectDeferred(new FairyItem(game::Config::fairy_Spawn_Position, this->current_level));
        fairy_has_spawned = true;
    }

    if (!mp.HasItem())
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
        this->score += enemy->Get_Score_Value();
        this->souls += enemy->Get_Souls_Value();

        // Nur die Position für später speichern
        dead_enemy_positions.push_back(enemy->Get_Position());
    }
});


// 2. VERARBEITEN: Nachdem das Aufräumen FERTIG ist, hier sicher die Item-Logik ausführen.
for (const auto& pos : dead_enemy_positions)
{
    if (GetRandomValue(1, 100) <= game::Config::enemy_Item_Drop_Chance_Percent)
    {
        // Hier ist der Aufruf von CountItemsOfType jetzt 100% sicher!
        std::vector<std::pair<ItemType, int>> weighted_list;
        int total_weight = 0;

        if (CountItemsOfType(ItemType::HEALTH_POTION, objectManager, mp) < game::Config::health_Potion_Max_On_Map) {
            weighted_list.push_back({ItemType::HEALTH_POTION, game::Config::item_Drop_Weight_Heal});
            total_weight += game::Config::item_Drop_Weight_Heal;
        }
        if (CountItemsOfType(ItemType::BOMB, objectManager, mp) < game::Config::bomb_Max_On_Map) {
            weighted_list.push_back({ItemType::BOMB, game::Config::item_Drop_Weight_Bomb});
            total_weight += game::Config::item_Drop_Weight_Bomb;
        }
        if (CountItemsOfType(ItemType::TESTO_NEEDLE, objectManager, mp) < game::Config::testo_Needle_Max_On_Map) {
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
                        case ItemType::HEALTH_POTION: spawned_item = new HealthPotion(pos); break;
                        case ItemType::BOMB: spawned_item = new BombItem(pos); break;
                        case ItemType::TESTO_NEEDLE: spawned_item = new TestoNeedle(pos); break;
                    }
                    if (spawned_item) {
                        // Und natürlich die sichere, verzögerte Methode zum Hinzufügen verwenden!
                        objectManager.AddObjectDeferred(spawned_item);
                    }
                    break;
                }
            }
        }
    }
}


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

    int playerHealth = static_cast<int>(mp.Get_Health());
    std::string healthText = "Leben: " + std::to_string(playerHealth);
    DrawText(healthText.c_str(), 20, 20, 30, WHITE);

    std::string scoreText = "Score: " + std::to_string(score);
    DrawText(scoreText.c_str(), 220, 20, 30, WHITE);

    std::string soulsText = "Souls: " + std::to_string(souls);
    DrawText(soulsText.c_str(), 450, 20, 30, WHITE);

    int time_to_wave = static_cast<int>(wave_timer / 10.0f);
    std::string waveText = "Naechste Welle in: " + std::to_string(time_to_wave);
    DrawText(waveText.c_str(), GetScreenWidth() - 400, 20, 30, WHITE);
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
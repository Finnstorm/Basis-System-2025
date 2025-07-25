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
    this->wave_timer = 3.0f;
}
game::scenes::GameScene::~GameScene()
{
}

void game::scenes::GameScene::Update()
{
    if (mp.Is_Dead())
    {
        auto newMenuScene = std::make_shared<game::scenes::MenuScene>();
        game::core::Store::stage->SwitchToNewScene("MenuScene", newMenuScene);
        return;
    }
    wave_timer -= dtm.Get_Dt();
    if (wave_timer <= 0.0f)
    {
        current_wave++;
        enemySpawner->Start_New_Wave(current_wave, current_level);
        wave_timer = game::Config::kWaveInterval;
    }

    enemySpawner->Update(dtm.Get_Dt());
    mp.Player_Input();
    std::vector<enemy::Enemy_Base_Class*> all_enemies;
    for (auto* object : objectManager.managed_objects) {
        if (auto* enemy = dynamic_cast<enemy::Enemy_Base_Class*>(object)) {
            all_enemies.push_back(enemy);
        }
    }

    Vector2 player_center = mp.Get_Player_Center();
    for (auto* object : objectManager.managed_objects) {
        object->Tick(dtm.Get_Dt());

        if (auto* enemy = dynamic_cast<enemy::Enemy_Base_Class*>(object))
        {
            enemy->Tick_AI(dtm.Get_Dt(), player_center, all_enemies);

            if (auto* melee_enemy = dynamic_cast<enemy::Melee_Enemy*>(enemy)) {
                melee_enemy->Tick_Melee(dtm.Get_Dt(), player_center);
            }
        }
    }

    p_cm->Check_Collisions();
    cam->Cam_Movement(dtm.Get_Dt(), screen.Get_Map_Dimensions());
    objectManager.Cleanup_Objects();
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
}
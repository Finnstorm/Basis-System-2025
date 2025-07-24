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
    cam = std::make_shared<Cam>(this->mp);

    screen.Load_Game_Objects(objectManager);

    mp.object_manager_ptr = &objectManager;
    p_cm = std::make_unique<Collision_Manager>(wb, objectManager.managed_objects);

    enemySpawner = std::make_unique<EnemySpawner>(objectManager, cam);

    Vector2 mapDims = screen.Get_Map_Dimensions();

    int initialEnemies = 3;
    enemySpawner->SpawnEnemies(initialEnemies, mapDims);

    this->enemiesPerWave = game::Config::kEnemySpawn;
    this->waveTimer = this->waveInterval;
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
    waveTimer -= dtm.Get_Dt();

    if (waveTimer <= 0.0f)
    {
        Vector2 mapDims = screen.Get_Map_Dimensions();
        enemySpawner->SpawnEnemies(this->enemiesPerWave, mapDims);
        this->enemiesPerWave += 5;
        this->waveTimer = this->waveInterval;
    }

    mp.Player_Input();

    Vector2 player_center = mp.Get_Player_Center();

    // NEU: Erstellen Sie eine Liste, die nur die Gegner enthält.
    std::vector<enemy::Enemy_Base_Class*> all_enemies;
    for (auto* object : objectManager.managed_objects) {
        if (auto* enemy = dynamic_cast<enemy::Enemy_Base_Class*>(object)) {
            all_enemies.push_back(enemy);
        }
    }

    // Jetzt iterieren und die Tick-Funktionen aufrufen
    for (auto* object : objectManager.managed_objects) {
        if (auto* enemy = dynamic_cast<enemy::Enemy_Base_Class*>(object)) {
            if (auto* melee_enemy = dynamic_cast<enemy::Melee_Enemy*>(enemy)) {
                // Übergeben Sie die neue Liste als viertes Argument
                melee_enemy->Tick(dtm.Get_Dt(), player_center.x, player_center.y, all_enemies);
            }
            // HINWEIS: Falls Sie andere Gegnertypen haben, die eine andere Tick-Funktion benötigen,
            // müssten Sie hier weitere `else if` Blöcke einfügen.
        } else {
            // Dies ruft Tick für Objekte auf, die keine Gegner sind (z.B. Spieler)
            object->Tick(dtm.Get_Dt());
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
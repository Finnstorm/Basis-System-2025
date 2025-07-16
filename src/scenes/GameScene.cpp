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
#include "../game/PlayerClassOne.h"
#include "../core/CollisionManager.h"
#include "../game/MeleeEnemy.h"


using namespace std::string_literals;

game::scenes::GameScene::GameScene()
{
    dtm.Start();
    objectManager.AddObject(&mp);
    cam = std::make_shared<Cam>(this->mp);

    screen.Load_Game_Objects(objectManager);

    mp.object_manager_ptr = &objectManager;
    p_cm = std::make_unique<Collision_Manager>(wb, objectManager.managed_objects);

    enemySpawner = std::make_unique<EnemySpawner>(objectManager, cam);

    // Hole die Kartendimensionen von deiner Screen-Klasse
    Vector2 mapDims = screen.Get_Map_Dimensions();

    // Übergib die Dimensionen als zweites Argument
    int initialEnemies = 3;
    enemySpawner->SpawnEnemies(initialEnemies, mapDims);

    // --- VARIABLEN FÜR DIE NÄCHSTE WELLE VORBEREITEN ---
    this->enemiesPerWave = game::Config::kEnemySpawn; // Die nächste Welle hat 8 Gegner
    this->waveTimer = this->waveInterval;
}

game::scenes::GameScene::~GameScene()
{
}

void game::scenes::GameScene::Update()
{
    if (mp.Is_Dead())
    {
        this->is_finished = true;
        this->next_scene_name = "MenuScene";
        return;
    }

    waveTimer -= dtm.Get_Dt();

    if (waveTimer <= 0.0f)
    {
        // ... (Wellen-Logik)
    }
    waveTimer -= dtm.Get_Dt(); // Zähle den Timer runter

    // Prüfen, ob es Zeit für eine neue Welle ist
    if (waveTimer <= 0.0f)
    {
        Vector2 mapDims = screen.Get_Map_Dimensions();

        // Spawne die nächste Welle
        enemySpawner->SpawnEnemies(this->enemiesPerWave, mapDims);

        // Bereite die übernächste Welle vor
        this->enemiesPerWave += 5; // Erhöhe die Gegnerzahl
        this->waveTimer = this->waveInterval; // Setze den Timer zurück
    }

    mp.Player_Input();

    Vector2 player_center = mp.Get_Player_Center();

    for (auto* object : objectManager.managed_objects) {
        if (auto* enemy = dynamic_cast<enemy::Enemy_Base_Class*>(object)) {
            if (auto* melee_enemy = dynamic_cast<enemy::Melee_Enemy*>(enemy)) {
                melee_enemy->Tick(dtm.Get_Dt(), player_center.x, player_center.y);
            }
        } else {
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
    // --- DEIN BISHERIGER ZEICHEN-CODE ---
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
    // --- ENDE DEINES BISHERIGEN CODES ---


    // --- NEUE LEBENSANZEIGE HINZUFÜGEN ---
    // Hole die aktuellen Lebenspunkte (als ganze Zahl für eine schönere Anzeige)
    int playerHealth = static_cast<int>(mp.Get_Health());

    // Erstelle den Text, der angezeigt werden soll
    std::string healthText = "Leben: " + std::to_string(playerHealth);

    // Zeichne den Text oben links auf den Bildschirm
    DrawText(healthText.c_str(), 20, 20, 30, WHITE); // Position (20,20), Schriftgröße 30, Farbe LIME
}
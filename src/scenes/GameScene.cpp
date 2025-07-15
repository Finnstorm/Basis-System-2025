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
    cam=std::make_shared<Cam>(this->mp);
    screen.Load_Game_Objects(objectManager);
    mp.object_manager_ptr = &objectManager;

    p_cm = std::make_unique<Collision_Manager>(wb, objectManager.managed_objects);
    cam = std::make_shared<Cam>(this->mp);
    screen.Load_Game_Objects(objectManager);
    auto* test_enemy = new enemy::Melee_Enemy({400, 400});
    objectManager.AddObject(test_enemy);
}

game::scenes::GameScene::~GameScene()
{
    // Your scene cleanup code here...
}

void game::scenes::GameScene::Update()
{
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
}
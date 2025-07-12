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

using namespace std::string_literals;

game::scenes::GameScene::GameScene()
{
    dtm.Start();
    objectManager.AddObject(&mp);
    cam=std::make_shared<Cam>(this->mp);
    screen.Load_Game_Objects(objectManager);

    // Your scene initialization code here...

}

game::scenes::GameScene::~GameScene()
{
    // Your scene cleanup code here...
}

void game::scenes::GameScene::Update()
{
    for (int i = 0; i < objectManager.managed_objects.size(); ++i) {
        objectManager.managed_objects[i]->Tick(dtm.Get_Dt());
    }

    this->p_cm->Check_Collisions();
    this->cam->Cam_Movement(dtm.Get_Dt(), screen.Get_Map_Dimensions());

    objectManager.Cleanup_Objects();
    dtm.Update();
}

void game::scenes::GameScene::Draw()
{
    BeginMode2D(this->cam->cam);
    screen.Draw_Level(this->cam, false);
    for(auto* obj : objectManager.managed_objects)
    {
        obj->Draw();
    }
    screen.Draw_Level(this->cam, true);
    EndMode2D();
}
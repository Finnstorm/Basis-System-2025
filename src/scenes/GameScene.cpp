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

#include "../game/PlayerClassOne.h"
#include "../core/CollisionManager.h"

using namespace std::string_literals;

game::scenes::GameScene::GameScene()
{
   {
    dtm.Start();
    cam=std::make_shared<Cam>(this->mp);
    screen.Load_Tiled_Objects(objectManager,p_cm);
    screen.Load_Tiled_Objects(objectManager);

    // Your scene initialization code here...

 void game::scenes::GameScene::Update()

    this->cam->Cam_Movement(dtm.Get_Dt());
    objectManager.Cleanup_Objects();
    p_cm->Check_Collisions();
    dtm.Update();
}

 void game::scenes::GameScene::Draw()
    BeginDrawing();
    ClearBackground(WHITE);
    screen.Draw_Level(this->cam);
    BeginMode2D(cam->cam);
    mp.Draw();

}

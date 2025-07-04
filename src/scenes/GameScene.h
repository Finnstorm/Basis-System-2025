#pragma once
#include "Scene.h"
#include "Screen.h"
#include "CollisionManager.h"
#include "../game/PlayerClassOne.h"
#include "DeltaTimeMachine.h"

namespace game::scenes
{
    class GameScene final : public game::core::Scene
    {private:
        Vector2 sp{100,100};
        Rectangle wb{0,0,game::Config::kStageWidth,game::Config::kStageHeight};
        Collision_Manager* p_cm;
        Player_Class_One mp{sp,p_cm};
        DT::timemachine dtm;
    public:
        GameScene();

        ~GameScene() override;

        void Update() override;

        void Draw() override;

        int Level_Nbr = 1;

        Screen screen = Screen(&Level_Nbr);

    private:
        // Add private members here...
    };
}
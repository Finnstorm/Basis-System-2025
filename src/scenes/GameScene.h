#pragma once
#include <Scene.h>
#include "Screen.h"
#include "CollisionManager.h"
#include "../game/PlayerClassOne.h"
#include "DeltaTimeMachine.h"
#include "Cam.h"
#include <memory>
#include "../game/EnemySpawner.h"
#include "../game/HUD.h"

namespace game::scenes
{
    class GameScene final : public game::core::Scene
    {
    public:
        Object_Manager objectManager;

    private:
        Vector2 sp = game::Config::player_Spawn_Position;
        Rectangle wb{0,0,game::Config::kStageWidth,game::Config::kStageHeight};
        Player_Class_One* player_ptr = nullptr;
        DT::timemachine dtm;
        std::unique_ptr<Collision_Manager> p_cm;
        std::unique_ptr<EnemySpawner> enemySpawner;
        int current_wave = 0;
        int current_level = 1;
        float wave_timer = 0.0f;

        bool fairy_has_spawned = false;
        float score_timer = 0.0f;
        std::string debug_text = "Debug Info:";

        HUD hud{nullptr};


    public:
        explicit GameScene(int level_to_load = 1);
        ~GameScene() override;
        void Update() override;
        void Draw() override;
        int CountItemsOfType(ItemType type, const Object_Manager& objectManager, const Player_Class_One& player);
        int Level_Nbr = 1;
        Screen screen {&Level_Nbr};
        std::shared_ptr<Cam> cam;
        bool is_finished = false;
        std::string next_scene_name;
    };
}
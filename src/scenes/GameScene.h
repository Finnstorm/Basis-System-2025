#pragma once
#include <Scene.h>
#include "Screen.h"
#include "CollisionManager.h"
#include "../game/PlayerClassOne.h"
#include "DeltaTimeMachine.h"
#include "Cam.h"
#include <memory>
#include "../game/EnemySpawner.h"

namespace game::scenes
{
    class GameScene final : public game::core::Scene
    {
    public:
        Object_Manager objectManager;

    private:
        Vector2 sp = game::Config::player_Spawn_Position;
        Rectangle wb{0,0,game::Config::kStageWidth,game::Config::kStageHeight};
        Player_Class_One mp{sp};
        DT::timemachine dtm;
        std::unique_ptr<Collision_Manager> p_cm;
        std::unique_ptr<EnemySpawner> enemySpawner;

        const float waveInterval = Config::kEnemySpawnTime;
        float waveTimer;
        int enemiesPerWave;

    public:
        GameScene();
        ~GameScene() override;
        void Update() override;
        void Draw() override;

        int Level_Nbr = 1;
        Screen screen {&Level_Nbr};
        std::shared_ptr<Cam> cam;
        bool is_finished = false;
        std::string next_scene_name;
    };
}
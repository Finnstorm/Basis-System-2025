#include "EnemySpawner.h"

#include <iostream>

#include "../core/Object_Manager.h"
#include "Cam.h"
#include "../Config.h.in"
#include <random>

EnemySpawner::EnemySpawner(Object_Manager& obj_manager, std::shared_ptr<Cam> main_cam)
    : objectManager(obj_manager), cam(main_cam)
{
}

void EnemySpawner::Register_Enemy_Type(const std::string& type_id, std::function<enemy::Enemy_Base_Class*(Vector2)> factory_func)
{
    enemy_factories[type_id] = factory_func;
    current_spawn_counts[type_id] = 0;
}

void EnemySpawner::Start_New_Wave(int wave_number, int level_number)
{
    wave_spawn_queue.clear();
    total_enemies_this_wave = 0;

    if (enemy_factories.count("Bauer"))
    {
        const int start_wave              = (level_number == 1) ? game::Config::kMeleeBauer_L1_StartWave : game::Config::kMeleeBauer_L2_StartWave;
        const int spawn_every_x_waves     = (level_number == 1) ? game::Config::kMeleeBauer_L1_SpawnEveryXWaves : game::Config::kMeleeBauer_L2_SpawnEveryXWaves;
        const int initial_spawn_count     = (level_number == 1) ? game::Config::kMeleeBauer_L1_InitialSpawnCount : game::Config::kMeleeBauer_L2_InitialSpawnCount;
        const int increase_per_spawn_wave = (level_number == 1) ? game::Config::kMeleeBauer_L1_IncreasePerSpawnWave : game::Config::kMeleeBauer_L2_IncreasePerSpawnWave;
        const bool enable_decrease        = (level_number == 1) ? game::Config::kMeleeBauer_L1_EnableDecrease : game::Config::kMeleeBauer_L2_EnableDecrease;
        const int decrease_start_wave     = (level_number == 1) ? game::Config::kMeleeBauer_L1_DecreaseStartWave : game::Config::kMeleeBauer_L2_DecreaseStartWave;
        const int decrease_amount         = (level_number == 1) ? game::Config::kMeleeBauer_L1_DecreaseAmount : game::Config::kMeleeBauer_L2_DecreaseAmount;

        if (wave_number >= start_wave)
        {
            if ((wave_number - start_wave) % spawn_every_x_waves == 0)
            {
                int& count = current_spawn_counts["Bauer"];
                if (wave_number == start_wave)
                {
                    count = initial_spawn_count;
                }
                else
                {
                    if (enable_decrease && wave_number >= decrease_start_wave)
                    {
                        count -= decrease_amount;
                    }
                    else
                    {
                        count += increase_per_spawn_wave;
                    }
                }
                if (count < 0) count = 0;
                if (count > 0)
                {
                    wave_spawn_queue.push_back({"Bauer", count});
                    total_enemies_this_wave += count;
                }
            }
        }
    }
    // Hier könnten später Prüfungen für andere Gegnerfolgen.
}

void EnemySpawner::Update(float delta_time)
{
    if (wave_spawn_queue.empty() || total_enemies_this_wave == 0)
    {
        return;
    }

    time_until_next_spawn -= delta_time;

    if (time_until_next_spawn <= 0.0f)
    {
        int task_index = GetRandomValue(0, wave_spawn_queue.size() - 1);
        SpawnTask& task = wave_spawn_queue[task_index];

        if (enemy_factories.count(task.enemy_type_id))
        {
            Vector2 spawn_pos = Get_Random_Spawn_Position();
            enemy::Enemy_Base_Class* new_enemy = enemy_factories[task.enemy_type_id](spawn_pos);
            objectManager.AddObject(new_enemy);
        }
        task.count_to_spawn--;
        if (task.count_to_spawn <= 0)
        {
            wave_spawn_queue.erase(wave_spawn_queue.begin() + task_index);
        }
        time_until_next_spawn = game::Config::kWaveSpawnDuration / total_enemies_this_wave;
    }
}

Vector2 EnemySpawner::Get_Random_Spawn_Position() const
{
    float screen_width = GetScreenWidth();
    float screen_height = GetScreenHeight();
    float offset = 50.0f;
    Vector2 camera_target = cam->cam.target;
    float zoom = cam->cam.zoom;

    float world_view_width = screen_width / zoom;
    float world_view_height = screen_height / zoom;

    Vector2 top_left = { camera_target.x - world_view_width / 2, camera_target.y - world_view_height / 2 };

    int side = GetRandomValue(0, 3);
    Vector2 spawn_pos;

    switch (side) {
        case 0:
            spawn_pos = { (float)GetRandomValue(top_left.x, top_left.x + world_view_width), top_left.y - offset };
            break;
        case 1:
            spawn_pos = { top_left.x + world_view_width + offset, (float)GetRandomValue(top_left.y, top_left.y + world_view_height) };
            break;
        case 2:
            spawn_pos = { (float)GetRandomValue(top_left.x, top_left.x + world_view_width), top_left.y + world_view_height + offset };
            break;
        case 3:
            spawn_pos = { top_left.x - offset, (float)GetRandomValue(top_left.y, top_left.y + world_view_height) };
            break;
    }
    return spawn_pos;
}

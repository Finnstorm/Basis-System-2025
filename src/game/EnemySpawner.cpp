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
        bool is_enabled = false;
        switch (level_number)
        {
            case 1: is_enabled = game::Config::kMeleeBauer_L1_IsEnabled; break;
            case 2: is_enabled = game::Config::kMeleeBauer_L2_IsEnabled; break;
            case 3: is_enabled = game::Config::kMeleeBauer_L3_IsEnabled; break;
        }

        if (is_enabled)
        {

            int start_wave;
            int spawn_every_x_waves;
            int initial_spawn_count;
            int increase_per_spawn_wave;
            bool enable_decrease;
            int decrease_start_wave;
            int decrease_amount;

            switch (level_number)
            {
                case 1:
                start_wave              = game::Config::kMeleeBauer_L1_StartWave;
                spawn_every_x_waves     = game::Config::kMeleeBauer_L1_SpawnEveryXWaves;
                initial_spawn_count     = game::Config::kMeleeBauer_L1_InitialSpawnCount;
                increase_per_spawn_wave = game::Config::kMeleeBauer_L1_IncreasePerSpawnWave;
                enable_decrease         = game::Config::kMeleeBauer_L1_EnableDecrease;
                decrease_start_wave     = game::Config::kMeleeBauer_L1_DecreaseStartWave;
                decrease_amount         = game::Config::kMeleeBauer_L1_DecreaseAmount;
                break;
                case 2:
                    start_wave              = game::Config::kMeleeBauer_L2_StartWave;
                spawn_every_x_waves     = game::Config::kMeleeBauer_L2_SpawnEveryXWaves;
                initial_spawn_count     = game::Config::kMeleeBauer_L2_InitialSpawnCount;
                increase_per_spawn_wave = game::Config::kMeleeBauer_L2_IncreasePerSpawnWave;
                enable_decrease         = game::Config::kMeleeBauer_L2_EnableDecrease;
                decrease_start_wave     = game::Config::kMeleeBauer_L2_DecreaseStartWave;
                decrease_amount         = game::Config::kMeleeBauer_L2_DecreaseAmount;
                break;
                case 3:
                    start_wave              = game::Config::kMeleeBauer_L3_StartWave;
                spawn_every_x_waves     = game::Config::kMeleeBauer_L3_SpawnEveryXWaves;
                initial_spawn_count     = game::Config::kMeleeBauer_L3_InitialSpawnCount;
                increase_per_spawn_wave = game::Config::kMeleeBauer_L3_IncreasePerSpawnWave;
                enable_decrease         = game::Config::kMeleeBauer_L3_EnableDecrease;
                decrease_start_wave     = game::Config::kMeleeBauer_L3_DecreaseStartWave;
                decrease_amount         = game::Config::kMeleeBauer_L3_DecreaseAmount;
                break;
            }

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
    }

if (enemy_factories.count("Ritter"))
    {
        bool is_enabled = false;
        switch (level_number)
        {
            case 1: is_enabled = game::Config::kMeleeRitter_L1_IsEnabled; break;
            case 2: is_enabled = game::Config::kMeleeRitter_L2_IsEnabled; break;
            case 3: is_enabled = game::Config::kMeleeRitter_L3_IsEnabled; break;
        }

        if (is_enabled)
        {
            int start_wave;
            int spawn_every_x_waves;
            int initial_spawn_count;
            int increase_per_spawn_wave;
            bool enable_decrease;
            int decrease_start_wave;
            int decrease_amount;

            switch (level_number)
            {
                case 1:
                start_wave              = game::Config::kMeleeRitter_L1_StartWave;
                spawn_every_x_waves     = game::Config::kMeleeRitter_L1_SpawnEveryXWaves;
                initial_spawn_count     = game::Config::kMeleeRitter_L1_InitialSpawnCount;
                increase_per_spawn_wave = game::Config::kMeleeRitter_L1_IncreasePerSpawnWave;
                enable_decrease         = game::Config::kMeleeRitter_L1_EnableDecrease;
                decrease_start_wave     = game::Config::kMeleeRitter_L1_DecreaseStartWave;
                decrease_amount         = game::Config::kMeleeRitter_L1_DecreaseAmount;
                break;
                case 2:
                start_wave              = game::Config::kMeleeRitter_L2_StartWave;
                spawn_every_x_waves     = game::Config::kMeleeRitter_L2_SpawnEveryXWaves;
                initial_spawn_count     = game::Config::kMeleeRitter_L2_InitialSpawnCount;
                increase_per_spawn_wave = game::Config::kMeleeRitter_L2_IncreasePerSpawnWave;
                enable_decrease         = game::Config::kMeleeRitter_L2_EnableDecrease;
                decrease_start_wave     = game::Config::kMeleeRitter_L2_DecreaseStartWave;
                decrease_amount         = game::Config::kMeleeRitter_L2_DecreaseAmount;
                break;
                case 3:
                start_wave              = game::Config::kMeleeRitter_L3_StartWave;
                spawn_every_x_waves     = game::Config::kMeleeRitter_L3_SpawnEveryXWaves;
                initial_spawn_count     = game::Config::kMeleeRitter_L3_InitialSpawnCount;
                increase_per_spawn_wave = game::Config::kMeleeRitter_L3_IncreasePerSpawnWave;
                enable_decrease         = game::Config::kMeleeRitter_L3_EnableDecrease;
                decrease_start_wave     = game::Config::kMeleeRitter_L3_DecreaseStartWave;
                decrease_amount         = game::Config::kMeleeRitter_L3_DecreaseAmount;
                break;
            }

            if (wave_number >= start_wave)
            {
                if ((wave_number - start_wave) % spawn_every_x_waves == 0)
                {
                    int& count = current_spawn_counts["Ritter"];
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
                        wave_spawn_queue.push_back({"Ritter", count});
                        total_enemies_this_wave += count;
                    }
                }
            }
        }
    }

    if (enemy_factories.count("Demonenritter"))
    {
        bool is_enabled = false;
        switch (level_number)
        {
            case 1: is_enabled = game::Config::kMeleeDemonenritter_L1_IsEnabled; break;
            case 2: is_enabled = game::Config::kMeleeDemonenritter_L2_IsEnabled; break;
            case 3: is_enabled = game::Config::kMeleeDemonenritter_L3_IsEnabled; break;
        }

        if (is_enabled)
        {

            int start_wave;
            int spawn_every_x_waves;
            int initial_spawn_count;
            int increase_per_spawn_wave;
            bool enable_decrease;
            int decrease_start_wave;
            int decrease_amount;

            switch (level_number)
            {
                case 1:
                start_wave              = game::Config::kMeleeDemonenritter_L1_StartWave;
                spawn_every_x_waves     = game::Config::kMeleeDemonenritter_L1_SpawnEveryXWaves;
                initial_spawn_count     = game::Config::kMeleeDemonenritter_L1_InitialSpawnCount;
                increase_per_spawn_wave = game::Config::kMeleeDemonenritter_L1_IncreasePerSpawnWave;
                enable_decrease         = game::Config::kMeleeDemonenritter_L1_EnableDecrease;
                decrease_start_wave     = game::Config::kMeleeDemonenritter_L1_DecreaseStartWave;
                decrease_amount         = game::Config::kMeleeDemonenritter_L1_DecreaseAmount;
                break;
                case 2:
                start_wave              = game::Config::kMeleeDemonenritter_L2_StartWave;
                spawn_every_x_waves     = game::Config::kMeleeDemonenritter_L2_SpawnEveryXWaves;
                initial_spawn_count     = game::Config::kMeleeDemonenritter_L2_InitialSpawnCount;
                increase_per_spawn_wave = game::Config::kMeleeDemonenritter_L2_IncreasePerSpawnWave;
                enable_decrease         = game::Config::kMeleeDemonenritter_L2_EnableDecrease;
                decrease_start_wave     = game::Config::kMeleeDemonenritter_L2_DecreaseStartWave;
                decrease_amount         = game::Config::kMeleeDemonenritter_L2_DecreaseAmount;
                break;
                case 3:
                start_wave              = game::Config::kMeleeDemonenritter_L3_StartWave;
                spawn_every_x_waves     = game::Config::kMeleeDemonenritter_L3_SpawnEveryXWaves;
                initial_spawn_count     = game::Config::kMeleeDemonenritter_L3_InitialSpawnCount;
                increase_per_spawn_wave = game::Config::kMeleeDemonenritter_L3_IncreasePerSpawnWave;
                enable_decrease         = game::Config::kMeleeDemonenritter_L3_EnableDecrease;
                decrease_start_wave     = game::Config::kMeleeDemonenritter_L3_DecreaseStartWave;
                decrease_amount         = game::Config::kMeleeDemonenritter_L3_DecreaseAmount;
                break;
            }

            if (wave_number >= start_wave)
            {
                if ((wave_number - start_wave) % spawn_every_x_waves == 0)
                {
                    int& count = current_spawn_counts["Demonenritter"];
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
                        wave_spawn_queue.push_back({"Demonenritter", count});
                        total_enemies_this_wave += count;
                    }
                }
            }
        }
    }
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

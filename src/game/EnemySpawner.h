#ifndef SPAWNER_H
#define SPAWNER_H

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <memory>
#include "raylib.h"

// Vorwärtsdeklarationen, um Abhängigkeiten zu reduzieren
class Object_Manager;
class Cam;
namespace enemy { class Enemy_Base_Class; }

// Eine einfache Struktur, die speichert, welcher Gegnertyp wie oft gespawnt werden soll.
struct SpawnTask
{
    std::string enemy_type_id;
    int count_to_spawn;
};

class EnemySpawner
{
private:
    Object_Manager& objectManager;
    std::shared_ptr<Cam> cam;
    std::vector<SpawnTask> wave_spawn_queue;
    float time_until_next_spawn = 0.0f;
    int total_enemies_this_wave = 0;

    std::map<std::string, std::function<enemy::Enemy_Base_Class*(Vector2)>> enemy_factories;
    std::map<std::string, int> current_spawn_counts;

public:
    EnemySpawner(Object_Manager& obj_manager, std::shared_ptr<Cam> main_cam);
    void Register_Enemy_Type(const std::string& type_id, std::function<enemy::Enemy_Base_Class*(Vector2)> factory_func);
    void Start_New_Wave(int wave_number, int level_number);
    void Update(float delta_time);

private:
    Vector2 Get_Random_Spawn_Position() const;
};
#endif




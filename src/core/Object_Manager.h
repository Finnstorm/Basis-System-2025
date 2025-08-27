#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include "../game/Collidable.h"
#include "../game/Walls.h"
#include "../game/EnemyBaseClass.h"
#include "../game/Consumables.h"
#include "../game/PlayerProjectile.h"
#include "../game/EnemyProjectile.h"
#include "../game/PlayerClassOne.h"

class Object_Manager
{
public:
    //öffentliche Vektor, der alle vom Manager verwalteten Collidable-Objekte speichert.
    std::vector<Collidable*> managed_objects;
    void AddObjectDeferred(Collidable* object);
    void ProcessAdditions();

public:

    Object_Manager();
    void AddObject(Collidable* object);
    void RemoveObject(Collidable* object);
    void ClearAllObjects();
    void Cleanup_Objects(const std::function<void(Collidable*)>& on_object_cleaned);
    std::vector<Collidable*> objects_to_add;

};
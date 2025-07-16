#ifndef SPAWNER_H
#define SPAWNER_H

#include <memory>
#include "../core/Object_Manager.h"
#include "Cam.h"
#include "raylib.h" // Nötig für Vector2

class EnemySpawner {
public:
    EnemySpawner(Object_Manager& objectManager, std::shared_ptr<Cam> camera);

    // WIR FÜGEN HIER mapDimensions HINZU
    void SpawnEnemies(int count, Vector2 mapDimensions);

private:
    Object_Manager& objectManagerRef;
    std::shared_ptr<Cam> camRef;

    // Wir erweitern diese Funktion später für schwieriges Gelände
    bool IsPositionValid(Vector2 position);
};
#endif




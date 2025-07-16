#include "EnemySpawner.h"
#include "MeleeEnemy.h"
#include "../config.h.in"
#include "raymath.h" // Wichtig für die Clamp-Funktion

EnemySpawner::EnemySpawner(Object_Manager& objectManager, std::shared_ptr<Cam> camera)
    : objectManagerRef(objectManager), camRef(camera) {}

bool EnemySpawner::IsPositionValid(Vector2 position) {
    Rectangle potentialHitbox = {
        position.x, position.y,
        game::Config::melee_enemy_1_hitbox.x,
        game::Config::melee_enemy_1_hitbox.y
    };

    for (Collidable* obj : objectManagerRef.managed_objects) {
        if (obj->Get_Collision_Type() == Collision_Type::WALL) {
            if (CheckCollisionRecs(potentialHitbox, obj->Get_Hitbox())) {
                return false;
            }
        }
    }


    return true;
}


void EnemySpawner::SpawnEnemies(int count, Vector2 mapDimensions) {
    if (!camRef || mapDimensions.x <= 0 || mapDimensions.y <= 0) return;

    Camera2D& currentCam = camRef->cam;
    float worldViewWidth = (float)GetScreenWidth() / currentCam.zoom;
    float worldViewHeight = (float)GetScreenHeight() / currentCam.zoom;

    Rectangle camView = {
        currentCam.target.x - (worldViewWidth / 2.0f),
        currentCam.target.y - (worldViewHeight / 2.0f),
        worldViewWidth,
        worldViewHeight
    };

    // Dieser Puffer bestimmt, wie weit außerhalb der Kamera gespawnt wird.
    // Ein größerer Wert bedeutet weiter weg.
    float spawnBuffer = 100.0f;
    Rectangle spawnArea = {
        camView.x - spawnBuffer, camView.y - spawnBuffer,
        camView.width + (spawnBuffer * 2), camView.height + (spawnBuffer * 2)
    };

    int enemiesSpawned = 0;
    int maxAttempts = 100;

    while (enemiesSpawned < count && maxAttempts > 0) {
        float randX = (float)GetRandomValue(spawnArea.x, spawnArea.x + spawnArea.width);
        float randY = (float)GetRandomValue(spawnArea.y, spawnArea.y + spawnArea.height);

        // --- NEUER SCHRITT: Position auf Kartengrenzen beschränken ---
        // Stellt sicher, dass die Koordinaten niemals außerhalb von (0,0) und (mapWidth, mapHeight) liegen.
        randX = Clamp(randX, 0.0f, mapDimensions.x - game::Config::melee_enemy_1_hitbox.x);
        randY = Clamp(randY, 0.0f, mapDimensions.y - game::Config::melee_enemy_1_hitbox.y);

        Vector2 spawnPos = {randX, randY};

        if (!CheckCollisionPointRec(spawnPos, camView) && IsPositionValid(spawnPos)) {
            objectManagerRef.AddObject(new enemy::Melee_Enemy(spawnPos));
            enemiesSpawned++;
        }
        maxAttempts--;
    }
}
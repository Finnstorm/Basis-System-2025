cmake_minimum_required(VERSION 3.11)

add_library(core STATIC game.h game.cpp store.h ../scenes/Scene.h stage.h stage.cpp renderer.h renderer.cpp texture2d.cpp texture2d.h sprite.h sprite.cpp SpriteAnimated.cpp SpriteAnimated.h actor.cpp actor.h
        DeltaTimeMachine.cpp
        DeltaTimeMachine.h
        Cam.cpp
        Cam.h
        ../game/EnemyBaseClass.cpp
        ../game/EnemyBaseClass.h
        ../game/Consumables.cpp
        ../game/Consumables.h
        ../game/Walls.cpp
        ../game/Walls.h
        ../game/Spawnpoints.cpp
        ../game/Spawnpoints.h
        ../game/Collidable.h
        CollisionManager.cpp
        CollisionManager.h
        Quadtree.cpp
        Quadtree.h
        ../game/PlayerClassOne.cpp
        ../game/PlayerClassOne.h
        ../game/PlayerBaseClass.cpp
        Animations.cpp
        Animations.h
        Object_Manager.cpp
        Object_Manager.h
        ../game/EnemyProjectile.cpp
        ../game/EnemyProjectile.h
        ../game/MeleeEnemy.cpp
        ../game/MeleeEnemy.h
        ../game/PlayerProjectile.cpp
        ../game/PlayerProjectile.h
        ../game/RangedEnemy8D.cpp
        ../game/RangedEnemy8D.h
        ../game/RangedEnemyOmni.cpp
        ../game/RangedEnemyOmni.h
        ../game/CollisionResponse.cpp
        ../game/CollisionResponse.h
        ../game/EnemySpawner.cpp
        ../game/EnemySpawner.h
        RepeatAnimation.cpp
        RepeatAnimation.h
        ../game/PlayerMeleeHitbox.cpp
        ../game/PlayerMeleeHitbox.h
        ../game/ItemBase.cpp
        ../game/ItemBase.h
        ../game/HealthPotion.cpp
        ../game/HealthPotion.h
        ../game/BombExplosionHitbox.cpp
        ../game/BombExplosionHitbox.h
        ../game/BombItem.cpp
        ../game/BombItem.h
        ../game/TestoNeedle.cpp
        ../game/TestoNeedle.h
        ../game/KeyItem.cpp
        ../game/KeyItem.h
        ../game/FairyItem.cpp
        ../game/FairyItem.h
        ../game/HUD.h
        ../game/HUD.cpp
        ../game/KnightEnemy.cpp
        ../game/KnightEnemy.h
        ../game/DemonKnightEnemy.cpp
        ../game/DemonKnightEnemy.h
        ../game/PeasantEnemy.cpp
        ../game/PeasantEnemy.h)

target_include_directories(core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(core raylib raylib_cpp nlohmann_json::nlohmann_json)

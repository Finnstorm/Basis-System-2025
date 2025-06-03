#pragma once

#include <memory>
#include <Playerinput.h>
#include <scene.h>


namespace game::scenes
{
    class GameScene final : public game::core::Scene {
    public:
        GameScene();

        ~GameScene() override;

        void Update() override;

        void Draw() override;

    private:
        // Pointer to the player object, managed by this scene
        std::unique_ptr<game::core::Player> up_player_;
    };
}
#pragma once
#include "Scene.h"

namespace game::scenes
{
    class PauseScene final : public game::core::Scene
    {
    public:
        PauseScene();

        ~PauseScene() override ;

        void Update() override;

        void Draw() override;
    };
}

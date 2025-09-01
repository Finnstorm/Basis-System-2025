//
// Created by $Will on 01.09.2025.
//

#ifndef UPGRADESCENE_H
#define UPGRADESCENE_H

#include "Scene.h"

namespace game::scenes
{
    class UpgradeScene final : public game::core::Scene
    {
    private:
        int souls_to_spend;
        int current_level;

    public:
        UpgradeScene(int souls, int level);
        ~UpgradeScene() override;
        void Update() override;
        void Draw() override;
    };
}

#endif //UPGRADESCENE_H

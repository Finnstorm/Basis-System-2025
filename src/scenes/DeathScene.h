//
// Created by $Will on 01.09.2025.
//

#ifndef DEATHSCENE_H
#define DEATHSCENE_H

#include "Scene.h"

namespace game::scenes
{
    class DeathScene final : public game::core::Scene
    {
    private:
        int final_score;
        int final_souls;
        bool has_fairy;
        int current_level;

    public:
        DeathScene(int score, int souls, bool fairy_status, int level);
        ~DeathScene() override;
        void Update() override;
        void Draw() override;
    };
}

#endif //DEATHSCENE_H

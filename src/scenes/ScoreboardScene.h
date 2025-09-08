//
// Created by $Will on 01.09.2025.
//

#ifndef SCOREBOARDSCENE_H
#define SCOREBOARDSCENE_H
#include "Scene.h"

namespace game::scenes
{
    class ScoreboardScene final : public game::core::Scene
    {
    private:
        int final_score;

    public:
        ScoreboardScene(int score);
        ~ScoreboardScene() override;
        void Update() override;
        void Draw() override;
    };
}


#endif //SCOREBOARDSCENE_H

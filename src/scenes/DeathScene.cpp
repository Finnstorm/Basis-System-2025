//
// Created by $Will on 01.09.2025.
//

#include "DeathScene.h"
#include "UpgradeScene.h"
#include "ScoreboardScene.h"
#include <raylib.h>
#include <Store.h>
#include "../config.h.in"
namespace game::scenes
{
    DeathScene::DeathScene(int score, int souls, bool fairy_status, int level)
    {
        this->final_score = score;
        this->final_souls = souls;
        this->has_fairy = fairy_status;
        this->current_level = level;
    }

    DeathScene::~DeathScene() {}

    void DeathScene::Update()
    {
        if (has_fairy && current_level < 3)
        {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(game::Config::key_Melee_Attack))
            {
                auto upgradeScene = std::make_shared<UpgradeScene>(final_souls, current_level);
                game::core::Store::stage->SwitchToNewScene("UpgradeScene", upgradeScene);
            }
        }

        if (IsKeyPressed(KEY_ESCAPE))
        {
            auto scoreboardScene = std::make_shared<ScoreboardScene>(final_score);
            game::core::Store::stage->SwitchToNewScene("ScoreboardScene", scoreboardScene);
        }
    }

    void DeathScene::Draw()
    {
        ClearBackground(BLACK);

        std::string fairy_text = has_fairy ? "Du hast das Feenitem." : "Du hast das Feenitem nicht.";
        std::string score_text = "Das ist der Todesscreen. " + fairy_text + " Dein Score betraegt " + std::to_string(final_score);

        DrawText(score_text.c_str(), 50, 200, 20, WHITE);

        if (has_fairy && current_level < 3)
        {
            DrawText("Enter oder Angriff für Upgradescreen", 50, 250, 20, GREEN);
            DrawText("ESC für Scoreboard Screen", 50, 280, 20, RED);
        }
        else
        {
            DrawText("ESC für Scoreboard Screen", 50, 280, 20, RED);
        }
    }
}

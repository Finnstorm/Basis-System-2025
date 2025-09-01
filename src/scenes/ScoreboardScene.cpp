//
// Created by $Will on 01.09.2025.
//

#include "ScoreboardScene.h"
#include "MainMenuScene.h"
#include <raylib.h>
#include <Store.h>
#include "../config.h.in"
namespace game::scenes
{
    ScoreboardScene::ScoreboardScene(int score)
    {
        this->final_score = score;
    }

    ScoreboardScene::~ScoreboardScene() {}

    void ScoreboardScene::Update()
    {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(game::Config::key_Melee_Attack))
        {
            auto mainMenu = std::make_shared<MainMenuScene>();
            game::core::Store::stage->SwitchToNewScene("MainMenu", mainMenu);
        }
    }

    void ScoreboardScene::Draw()
    {
        ClearBackground(BLUE);
        std::string score_text = "Sie haben folgenden Score: " + std::to_string(final_score);

        DrawText(score_text.c_str(), 50, 200, 20, WHITE);
        DrawText("Stellen Sie sich einfach vor, er wuerde hier an der Stelle", 50, 230, 20, WHITE);
        DrawText("ganz weit oben auf einem Scoreboard stehen.", 50, 260, 20, WHITE);
        DrawText("Druecken Sie Enter/Angriff um zurueck ins Menue zu gelangen", 50, 310, 20, GREEN);
    }
}
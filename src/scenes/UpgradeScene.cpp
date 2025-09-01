//
// Created by $Will on 01.09.2025.
//

#include "UpgradeScene.h"
#include "GameScene.h"
#include <raylib.h>
#include <Store.h>
#include "../config.h.in"

namespace game::scenes
{
    UpgradeScene::UpgradeScene(int souls, int level)
    {
        this->souls_to_spend = souls;
        this->current_level = level;
    }

    UpgradeScene::~UpgradeScene() {}

    void UpgradeScene::Update()
    {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(game::Config::key_Melee_Attack))
        {
            int next_level = current_level + 1;
            auto newGameScene = std::make_shared<GameScene>(next_level);
            game::core::Store::stage->SwitchToNewScene("GameScene", newGameScene);
        }
    }

    void UpgradeScene::Draw()
    {
        ClearBackground(DARKGRAY);
        std::string souls_text = "Das ist das Upgrade Screen und du hast " + std::to_string(souls_to_spend) + " Seelen.";

        DrawText(souls_text.c_str(), 50, 200, 20, WHITE);
        DrawText("Nur kannst du leider noch nichts upgraden.", 50, 230, 20, WHITE);
        DrawText("Druecke Angriff oder Enter um weiter zu kommen.", 50, 280, 20, GREEN);
    }
}

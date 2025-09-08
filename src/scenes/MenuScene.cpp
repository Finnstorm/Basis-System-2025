#include "MenuScene.h"
#include <string>
#include <raylib.h>
#include <Store.h>
#include "GameScene.h"
#include "../Vectors.h"

using namespace std::string_literals;

game::scenes::MenuScene::MenuScene() { }
game::scenes::MenuScene::~MenuScene() { }

void game::scenes::MenuScene::Update()
{
    if (IsKeyPressed(KEY_ENTER))
    {
        game::core::Store::last_score = 0;
        game::core::Store::stage->ReplaceWithNewScene("menu"s, "game"s, std::make_unique<GameScene>());
    }
}

void game::scenes::MenuScene::Draw()
{
    std::string menuText;
    if (game::core::Store::last_score > 0)
    {
        menuText = "Dein Score betraegt " + std::to_string(game::core::Store::last_score) + ". Um es erneut zu versuchen, druecke Enter.";
    }
    else
    {
        menuText = "Druecke Enter, um das Spiel zu starten";
    }

    // Text zentriert auf dem Bildschirm zeichnen
    int fontSize = 40;
    int textWidth = MeasureText(menuText.c_str(), fontSize);
    DrawText(menuText.c_str(), GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - (fontSize / 2), fontSize, RED);
}


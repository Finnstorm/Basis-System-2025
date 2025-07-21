#include "MenuScene.h"
#include <string>
#include <raylib.h>
#include "Screen.h"
#include <Store.h>
#include "GameScene.h"
#include "../Vectors.h"

using namespace std::string_literals;

game::scenes::MenuScene::MenuScene() { }

game::scenes::MenuScene::~MenuScene() { }

void game::scenes::MenuScene::Update()
{
    if (IsKeyPressed(KEY_ENTER))
        game::core::Store::stage->ReplaceWithNewScene("menu"s, "game"s, std::make_unique<GameScene>());
}

void game::scenes::MenuScene::Draw() { }


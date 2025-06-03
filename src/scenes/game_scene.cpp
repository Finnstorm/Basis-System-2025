#include <memory>
#include <string>
#include "../config.h.in"
#include <raylib.h>
#include <store.h>
#include "scenes.h"
#include "renderer.h"
#include "game_scene.h"

using namespace std::string_literals;

game::scenes::GameScene::GameScene()
{
    /* Your scene initialization code here...
    std::shared_ptr<game::core::Actor> actor1 = std::make_unique<game::core::Actor>(std::make_unique<game::core::Sprite>(std::make_shared<game::core::Texture2D>("assets/graphics/ball.png"), 100, 100));
    this->actors.insert(std::make_pair("actor1", actor1));

    std::shared_ptr<game::core::Actor> actor2 = std::make_unique<game::core::Actor>(std::make_unique<game::core::SpriteAnimated>(std::make_shared<game::core::Texture2D>("assets/graphics/anim_sprite.png"), 80.0f, 80.0f, 1, 3, 50, 100, 300));
    this->actors.insert(std::make_pair("actor2", actor2));*/

    up_player_ = std::make_unique<game::core::Player>
            (Vector2{game::Config::player_Start_X, game::Config::player_Start_Y},
            game::Config::player_Movement_Speed); // Erschaffe Spieler aus Config
}

game::scenes::GameScene::~GameScene() {
    // Your scene cleanup code here...
}

void game::scenes::GameScene::Update()
{
    // Your process input and update game scene code here...

    // Update the player logic (input handling, movement, attack state)
    if (up_player_ != nullptr) // Sicherstellen, dass der Player existiert
    {
        up_player_->Update();
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        game::core::Store::stage->switchToNewScene("pause"s, std::make_unique<PauseScene>());
    }
}

void game::scenes::GameScene::Draw()
{
    // Draw the player
    if (up_player_ != nullptr) // Sicherstellen, dass der Player existiert
    {
        up_player_->Draw();
    }

    DrawText("This is the game scene - press ESCAPE for pause", 10, 10, 30, LIGHTGRAY);
}

#include <memory>
#include <string>
#include "../config.h.in"
#include <raylib.h>
#include <store.h>
#include "scenes.h"
#include "game_scene.h"
#include <iostream>
#include "../core/collision_check.h"

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
    if (up_player_ != nullptr)
    {
        // 1. Berechne die beabsichtigte Bewegung für diesen Frame
        Vector2 intended_movement = up_player_->Update(); // Ruft die modifizierte Player::Update() auf

        // 2. Hole aktuelle Position und Größe des Spielers
        Vector2 current_position = up_player_->Get_Position();
        float player_w = game::Config::player_Width;    // Gemäß deinem Styleguide player_Width
        float player_h = game::Config::player_Height;   // Gemäß deinem Styleguide player_Height

        // Temporäre neue Position für X-Achsen-Bewegung
        Vector2 position_after_x_move = current_position;
        position_after_x_move.x += intended_movement.x;
        up_player_->Set_Position(position_after_x_move); // Spieler erstmal auf X-Achse bewegen

        Rectangle player_rect_x = {
            position_after_x_move.x - player_w / 2.0f,
            current_position.y - player_h / 2.0f, // Y-Position ist noch die alte
            player_w,
            player_h
        };

        // 3. Kollisionsprüfung und -behandlung für die X-Achse
        for (const auto& wall : game_map_screen_.walls)
        {
            if (game::core::check_collision(player_rect_x, wall.GetRect()))
            {
                std::cout << "Collision X with wall!" << std::endl;
                // Kollision auf X-Achse
                if (intended_movement.x > 0) // Bewegt sich nach rechts
                {
                    // Spieler an der linken Kante der Wand positionieren
                    position_after_x_move.x = wall.GetRect().x - player_w / 2.0f - 0.01f; // kleiner Offset
                }
                else if (intended_movement.x < 0) // Bewegt sich nach links
                {
                    // Spieler an der rechten Kante der Wand positionieren
                    position_after_x_move.x = wall.GetRect().x + wall.GetRect().width + player_w / 2.0f + 0.01f; // kleiner Offset
                }
                up_player_->Set_Position(position_after_x_move); // Korrigierte X-Position setzen
                break; // Nur eine Kollision pro Achse pro Frame auflösen
            }
        }

        // Aktuelle Position nach X-Kollisionsbehandlung holen (könnte sich geändert haben)
        Vector2 position_after_y_move = up_player_->Get_Position();
        position_after_y_move.y += intended_movement.y;
        up_player_->Set_Position(position_after_y_move); // Spieler erstmal auf Y-Achse bewegen

        Rectangle player_rect_y = {
            position_after_y_move.x - player_w / 2.0f, // X-Position ist die (ggf. korrigierte) von oben
            position_after_y_move.y - player_h / 2.0f,
            player_w,
            player_h
        };

        // 4. Kollisionsprüfung und -behandlung für die Y-Achse
        for (const auto& wall : game_map_screen_.walls)
        {
            if (game::core::check_collision(player_rect_y, wall.GetRect()))
            {
                std::cout << "Collision Y with wall!" << std::endl;
                // Kollision auf Y-Achse
                if (intended_movement.y > 0) // Bewegt sich nach unten
                {
                    // Spieler an der oberen Kante der Wand positionieren
                    position_after_y_move.y = wall.GetRect().y - player_h / 2.0f - 0.01f;
                }
                else if (intended_movement.y < 0) // Bewegt sich nach oben
                {
                    // Spieler an der unteren Kante der Wand positionieren
                    position_after_y_move.y = wall.GetRect().y + wall.GetRect().height + player_h / 2.0f + 0.01f;
                }
                up_player_->Set_Position(position_after_y_move); // Korrigierte Y-Position setzen
                break; // Nur eine Kollision pro Achse pro Frame auflösen
            }
        }
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        game::core::Store::stage->switchToNewScene("pause"s, std::make_unique<PauseScene>());
    }
}

void game::scenes::GameScene::Draw()
{
    game_map_screen_.draw_Level();
    // Draw the player
    if (up_player_ != nullptr) // Sicherstellen, dass der Player existiert
    {
        up_player_->Draw();
    }

    DrawText("This is the game scene - press ESCAPE for pause", 10, 10, 30, LIGHTGRAY);
}

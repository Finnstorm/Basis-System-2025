#include <memory>
#include <string>
#include "../config.h.in"
#include <raylib.h>
#include <store.h>
#include "scenes.h"
#include "renderer.h"
#include "game_scene.h"
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
    if (up_player_ != nullptr) {
        Vector2 old_player_position = up_player_->Get_Position(); // Alte Position speichern

        up_player_->Update(); // Spieler bewegt sich basierend auf Input

        Rectangle player_rect = {
            up_player_->Get_Position().x - game::Config::player_Width / 2.0f,
            up_player_->Get_Position().y - game::Config::player_Height / 2.0f,
            game::Config::player_Width,
            game::Config::player_Height
        };

        // Kollisionsprüfung mit allen Wänden
        for (const auto& wall : game_map_screen_.walls) { // Zugriff auf walls in unserem Screen-Objekt
            if (game::core::check_collision(player_rect, wall.GetRect())) {
                // Kollision gefunden! Setze Spieler auf alte Position zurück.
                // Bessere Kollisionsauflösung wäre, den Spieler nur in der Achse zurückzusetzen,
                // in der die Kollision primär stattfand, um "Hängenbleiben" zu ermöglichen.
                // Aber für den Anfang ist das Zurücksetzen der einfachste Weg.

                // up_player_->Set_Position(old_player_position); // Du bräuchtest eine Set_Position Methode im Player

                // Da Playerinput.cpp keine Set_Position hat, müssen wir es anders machen:
                // Wir verhindern die Bewegung, indem wir die aktuelle Position so anpassen,
                // dass sie der alten entspricht, oder genauer: die Überlappung auflösen.
                // Für den allerersten, einfachsten Test:
                // Wir können die Logik in Player::Update so anpassen, dass sie die Wände direkt prüft,
                // oder wir geben Player::Update eine Referenz auf die Wände.
                // Oder, für jetzt am einfachsten, wir implementieren das Zurücksetzen hier rudimentär,
                // indem wir die Bewegungskomponente, die zur Kollision führte, negieren.
                // Das ist aber unsauber.

                // IDEALERWEISE: Playerinput.cpp würde eine Methode haben wie:
                // player_resolve_collision(const Rectangle& wall_rect, Vector2 old_pos)
                // Oder Playerinput::Update nimmt die Wände als Parameter.

                // ---- EINFACHSTE VARIANTE FÜR JETZT (aber nicht perfekt): ----
                // Wenn eine Kollision auftritt, setzen wir die Spielerkoordinaten direkt zurück.
                // Dafür bräuchten wir eine Methode wie `up_player_->Set_Position(old_player_position);`
                // Da wir die nicht haben, ist dieser einfache Ansatz hier schwierig sauber umzusetzen.

                // ---- BESSERER ANSATZ (kleiner Umbau in Playerinput): ----
                // Wir lassen Playerinput::Update die Kollisionen selbst prüfen oder bekommen sie übergeben.
                // Für den Moment machen wir es hier in GameScene, indem wir die Position des Spielers
                // direkt manipulieren, wenn eine Set_Position Methode existiert.

                // --- Annahme: Du fügst Player::Set_Position hinzu ---
                // In Playerinput.h: void Set_Position(Vector2 new_position);
                // In Playerinput.cpp: void Player::Set_Position(Vector2 new_position) { this->current_position = new_position; }
                up_player_->Set_Position(old_player_position); // Wenn Set_Position existiert

                // Wichtig: Wenn mehrere Kollisionen in einem Frame passieren könnten, muss man das hier
                // eventuell robuster gestalten oder nach der ersten Kollision abbrechen.
                break; // Verlasse die Schleife nach der ersten erkannten Kollision für diesen Frame
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

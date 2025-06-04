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
    up_player_ = std::make_unique<game::core::Player>(
        Vector2{game::Config::player_Start_X, game::Config::player_Start_Y},
        game::Config::player_Movement_Speed
    );
    // game_map_screen_ wird automatisch mit seinem Standardkonstruktor initialisiert,
    // der Load_Object_Layers() aufruft und somit die 'walls' (inkl. Movable_Walls) lädt.
}

game::scenes::GameScene::~GameScene()
{
    // Dein Code zur Szenenbereinigung hier...
}

void game::scenes::GameScene::Update()
{
    // std::cout << "--- GameScene::Update() TOP --- Frame: " << game::core::Store::ticks << std::endl; // Kann für weniger Spam auskommentiert werden

    float delta_time = 0.0f;
    if (game::core::Store::dtm != nullptr) {
        delta_time = game::core::Store::dtm->Get_Dt();
    } else {
        // std::cout << "GameScene::Update() - WARNING: game::core::Store::dtm is nullptr!" << std::endl;
    }

    // 1. Update für alle Wände (Movable_Wall::Update kümmert sich um Cooldowns und Push-Delay-Timer)
    std::vector<game::core::Wall*> all_wall_pointers = game_map_screen_.Get_Wall_Pointers();
    for (const auto& wall_unique_ptr : game_map_screen_.walls) {
        if (wall_unique_ptr != nullptr) {
            wall_unique_ptr->Update(delta_time, all_wall_pointers);
        }
    }

    if (up_player_ == nullptr) {
        // std::cout << "GameScene::Update() - CRITICAL: up_player_ is nullptr. Aborting player logic." << std::endl;
        if (IsKeyPressed(KEY_ESCAPE)) { // Trotzdem Escape ermöglichen
            game::core::Store::stage->switchToNewScene("pause"s, std::make_unique<PauseScene>());
        }
        return;
    }
    // std::cout << "GameScene::Update() - Player is valid." << std::endl; // Kann auskommentiert werden

    Vector2 intended_movement = up_player_->Update();
    Vector2 current_player_position = up_player_->Get_Position();
    float player_w = game::Config::player_Width;
    float player_h = game::Config::player_Height;

    // --- X-Achsen Kollisionsbehandlung für Spieler ---
    Vector2 next_player_pos_x = {current_player_position.x + intended_movement.x, current_player_position.y};
    Rectangle player_rect_x = { next_player_pos_x.x - player_w / 2.0f, current_player_position.y - player_h / 2.0f, player_w, player_h };
    for (const auto& wall_unique_ptr : game_map_screen_.walls) {
        game::core::Wall* wall_ptr = wall_unique_ptr.get();
        if (!wall_ptr) continue;
        if (game::core::check_collision(player_rect_x, wall_ptr->Get_Rect())) {
            if (intended_movement.x > 0) next_player_pos_x.x = wall_ptr->Get_Rect().x - player_w / 2.0f - 0.01f;
            else if (intended_movement.x < 0) next_player_pos_x.x = wall_ptr->Get_Rect().x + wall_ptr->Get_Rect().width + player_w / 2.0f + 0.01f;
            break;
        }
    }
    up_player_->Set_Position({next_player_pos_x.x, current_player_position.y});
    current_player_position = up_player_->Get_Position();

    // --- Y-Achsen Kollisionsbehandlung für Spieler ---
    Vector2 next_player_pos_y = {current_player_position.x, current_player_position.y + intended_movement.y};
    Rectangle player_rect_y = { current_player_position.x - player_w / 2.0f, next_player_pos_y.y - player_h / 2.0f, player_w, player_h };
    for (const auto& wall_unique_ptr : game_map_screen_.walls) {
        game::core::Wall* wall_ptr = wall_unique_ptr.get();
        if (!wall_ptr) continue;
        if (game::core::check_collision(player_rect_y, wall_ptr->Get_Rect())) {
            if (intended_movement.y > 0) next_player_pos_y.y = wall_ptr->Get_Rect().y - player_h / 2.0f - 0.01f;
            else if (intended_movement.y < 0) next_player_pos_y.y = wall_ptr->Get_Rect().y + wall_ptr->Get_Rect().height + player_h / 2.0f + 0.01f;
            break;
        }
    }
    up_player_->Set_Position({current_player_position.x, next_player_pos_y.y});
    current_player_position = up_player_->Get_Position(); // Finale Spielerposition für diesen Frame


    // --- Logik für das tatsächliche Bewegen einer Movable_Wall durch den Spieler ---
    Rectangle final_player_rect = {
        current_player_position.x - player_w / 2.0f,
        current_player_position.y - player_h / 2.0f,
        player_w, player_h
    };
    game::core::Player::Direction player_intended_push_direction = up_player_->Get_Last_Direction();

    for (const auto& wall_unique_ptr : game_map_screen_.walls) {
        game::core::Wall* wall_ptr = wall_unique_ptr.get();
        if (!wall_ptr) continue;

        game::core::Movable_Wall* movable_wall = dynamic_cast<game::core::Movable_Wall*>(wall_ptr);

        if (movable_wall) {
            // DEBUG: Zustand der Movable_Wall, bevor wir die Push-Logik prüfen
            if (wall_ptr->Get_Type() == "Movable_Wall_1") { // Fokussiere Debug auf eine bestimmte Wand
                std::cout << "GameScene: Eval MW1. Solved=" << movable_wall->Is_Solved()
                          << ", Moving=" << movable_wall->Is_Currently_Moving()
                          << ", PushT=" << movable_wall->DEBUG_Get_Push_Timer() // NEUE DEBUG-METHODE NÖTIG
                          << ", CooldownT=" << movable_wall->DEBUG_Get_Cooldown_Timer() // NEUE DEBUG-METHODE NÖTIG
                          << std::endl;
            }

            if (!movable_wall->Is_Solved() && !movable_wall->Is_Currently_Moving()) {
                Rectangle current_movable_wall_rect = movable_wall->Get_Rect();

                // Verwende ein leicht vergrößertes Spieler-Rect für die "Touch"-Erkennung
                Rectangle expanded_player_rect = final_player_rect;
                float touch_expansion = 1.0f;
                expanded_player_rect.x -= touch_expansion;
                expanded_player_rect.y -= touch_expansion;
                expanded_player_rect.width += 2 * touch_expansion;
                expanded_player_rect.height += 2 * touch_expansion;
                bool is_touching = game::core::check_collision(expanded_player_rect, current_movable_wall_rect);

                bool is_pushing_key_in_wall_direction = false;
                if (is_touching) { // Nur prüfen, wenn wir nah genug dran sind / "berühren"
                    switch (player_intended_push_direction) {
                        case game::core::Player::Right:
                            if (IsKeyDown(game::Config::key_Right) && (current_movable_wall_rect.x > final_player_rect.x))
                                is_pushing_key_in_wall_direction = true;
                            break;
                        case game::core::Player::Left:
                            if (IsKeyDown(game::Config::key_Left) && (current_movable_wall_rect.x + current_movable_wall_rect.width < final_player_rect.x + final_player_rect.width))
                                is_pushing_key_in_wall_direction = true;
                            break;
                        case game::core::Player::Down:
                            if (IsKeyDown(game::Config::key_Down) && (current_movable_wall_rect.y > final_player_rect.y))
                                is_pushing_key_in_wall_direction = true;
                            break;
                        case game::core::Player::Up:
                            if (IsKeyDown(game::Config::key_Up) && (current_movable_wall_rect.y + current_movable_wall_rect.height < final_player_rect.y + final_player_rect.height))
                                is_pushing_key_in_wall_direction = true;
                            break;
                        default: break;
                    }
                }

                // Ausführliche Debug-Ausgabe für die Push-Bedingungen
                if (wall_ptr->Get_Type() == "Movable_Wall_1") {
                    std::cout << "  GS Check MW1: Touch(exp)=" << is_touching
                              << ", PushKeyDir=" << is_pushing_key_in_wall_direction
                              << ", WallRdyToMove(DelayDone&CooldownDone)=" << movable_wall->Is_Ready_To_Move()
                              << "\n    PlayerR: x=" << final_player_rect.x << ", y=" << final_player_rect.y
                              << "\n    WallR:   x=" << current_movable_wall_rect.x << ", y=" << current_movable_wall_rect.y
                              << std::endl << "  ---" << std::endl;
                }

                if (is_touching && is_pushing_key_in_wall_direction) {
                    // Der Spieler versucht aktiv, die Wand zu schieben.
                    // Movable_Wall::Update hat bereits den current_push_delay_timer_ aktualisiert.
                    // Movable_Wall::Start_Push_Attempt() wird hier NICHT MEHR aufgerufen,
                    // sondern nur, wenn der Spieler *beginnt* zu pushen (z.B. bei erster Kollision).
                    // Die aktuelle Start_Push_Attempt Logik ist dafür nicht ideal.

                    // NEUER ANSATZ:
                    // 1. Wenn Spieler berührt UND Taste drückt -> signalisiere "Push-Wunsch" an die Wand.
                    // 2. Die Wand startet intern ihren Push-Delay-Timer (wenn nicht schon aktiv).
                    // 3. Wenn der Push-Delay-Timer der Wand abgelaufen ist (in Movable_Wall::Update geprüft) UND
                    //    der Spieler immer noch aktiv pusht (Taste immer noch gedrückt und Berührung), DANN Attempt_Move.

                    // Für diesen Versuch: Wir rufen Start_Push_Attempt auf, das den Timer setzt, wenn er 0 ist.
                    // Und Is_Ready_To_Move prüft, ob er 0 ist. Das führt zum Ping-Pong.

                    // Wir müssen die Logik ändern:
                    // GameScene teilt der Wand mit, dass der Spieler pusht.
                    // Die Wand managt ihren Timer. Wenn der Timer abläuft UND der Spieler immer noch pusht, bewegt sich die Wand.

                    // Temporäre Änderung für Test:
                    // Wenn der Spieler pusht, und der Timer der Wand ist bereits abgelaufen (Is_Ready_To_Move),
                    // dann versuche zu bewegen. Wenn der Timer noch nicht abgelaufen ist, startet Start_Push_Attempt ihn (wenn er 0 war).

                    if (movable_wall->Is_Ready_To_Move()) { // Prüft, ob Push-Delay und Cooldown abgelaufen sind
                        std::cout << "  GameScene: Player pushing " << movable_wall->Get_Type()
                                  << " AND Wall is ReadyToMove. Calling Attempt_Move." << std::endl;
                        movable_wall->Attempt_Move(player_intended_push_direction, all_wall_pointers);
                    } else {
                        // Wenn nicht bereit zu bewegen, aber Spieler pusht, stelle sicher, dass der Push-Timer gestartet wird/läuft.
                        movable_wall->Start_Push_Attempt(); // Diese Methode startet den Timer, wenn er <=0 ist.
                                                            // Der Timer wird in Movable_Wall::Update heruntergezählt.
                        // std::cout << "  GameScene: Player pushing " << movable_wall->Get_Type()
                        //           << ", but wall NOT ReadyToMove (likely push delay or cooldown active)." << std::endl;
                    }
                }
            }
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        game::core::Store::stage->switchToNewScene("pause"s, std::make_unique<PauseScene>());
    }
}

void game::scenes::GameScene::Draw()
{
    game_map_screen_.draw_Level(); // Zeichnet Kacheln

    // Zeichne alle Wände (statische und bewegliche)
    for (const auto& wall_ptr : game_map_screen_.walls)
    {
        if (wall_ptr != nullptr)
        {
            wall_ptr->Draw(); // Ruft die Draw-Methode von Wall oder Movable_Wall auf
        }
    }

    if (up_player_ != nullptr)
    {
        up_player_->Draw();
    }

    // DrawText("This is the game scene - press ESCAPE for pause", 10, 10, 30, LIGHTGRAY);
}
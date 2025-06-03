#include <raylib.h>
#include "../scenes/screen.h"
#include "game.h"
#include "store.h"
#include "stage.h"
#include "../scenes/menu_scene.h"
#include "../Config.h.in"

using namespace std::string_literals;

game::core::Game::Game
    (int virtual_render_width, int virtual_render_height, // Parameter für die virtuelle Bühnengröße
    bool full_screen,
    int target_fps,
    int config_window_flags, // Die Flags aus der Config
    int texture_filter,
    int exit_key,
    bool mouse,
    bool audio,
    const char *project_name)
    : stage_width_(virtual_render_width),   // Speichert die virtuelle Breite (z.B. 512)
      stage_height_(virtual_render_height), // Speichert die virtuelle Höhe (z.B. 288)
      audio_(audio),
      mouse_(mouse)
{
    int init_flags = config_window_flags; // Start mit den Basis-Flags aus der Config

    if (full_screen)
    {
        // Füge das Vollbild-Flag hinzu, falls es nicht schon da ist
        // (Obwohl es besser ist, es hier explizit zu managen)
        init_flags |= FLAG_FULLSCREEN_MODE;
        // Alternativ: init_flags = FLAG_FULLSCREEN_MODE | FLAG_VSYNC_HINT | ... (nur die für Vollbild relevanten)

        // Für Vollbild: InitWindow mit 0,0 sollte die native Desktop-Auflösung verwenden.
        // Raylib setzt dann GetScreenWidth()/Height() auf die Desktop-Werte.
        SetConfigFlags(init_flags);
        InitWindow(0, 0, project_name);
    }
    else
    {
        // Für Fenstermodus: Verwende die dedizierten Fenstergrößen-Konstanten
        // Stelle sicher, dass FLAG_FULLSCREEN_MODE hier nicht gesetzt ist, falls es in config_window_flags war
        init_flags &= ~FLAG_FULLSCREEN_MODE; // Entferne Fullscreen-Flag, falls vorhanden
        init_flags |= FLAG_WINDOW_RESIZABLE; // Stelle sicher, dass Resizable gesetzt ist für Fenstermodus (optional)

        SetConfigFlags(init_flags);
        InitWindow(game::Config::kWindowedModeWidth, game::Config::kWindowedModeHeight, project_name);
    }

    // SetWindowMinSize ist eher für den Fenstermodus relevant, um zu verhindern,
    // dass das Fenster zu klein gezogen wird.
    // Die Mindestgröße könnte sich an der Hälfte der virtuellen Bühne orientieren.
    if (!full_screen) {
        SetWindowMinSize(this->stage_width_ / 2, this->stage_height_ / 2);
    }

    SetTargetFPS(target_fps);

    // Die RenderTexture wird IMMER mit der virtuellen Bühnengröße erstellt
    this->render_target_ = LoadRenderTexture(this->stage_width_, this->stage_height_);
    SetTextureFilter(this->render_target_.texture, texture_filter);

    SetExitKey(exit_key);

    // Die explizite ToggleFullscreen()-Funktion wird nicht mehr benötigt,
    // da InitWindow das bereits über die Flags handhaben sollte.
    // if (full_screen) ToggleFullscreen(); // ENTFERNEN

    if (audio)
        InitAudioDevice();

    if (!mouse)
        HideCursor();
}

game::core::Game::~Game() {
    TraceLog(LOG_INFO, "game::core::Game destructor called");

    // Release the stage object to trigger potential cleanups that may be dependent on code after the Run() function call.
    game::core::Store::stage = nullptr;

    if (this->audio_)
        CloseAudioDevice();

    // Unload render texture
    UnloadRenderTexture(this->render_target_);

    // Close window and OpenGL context
    CloseWindow();
}

void game::core::Game::Run(const std::string& scene_name, std::unique_ptr<game::core::Scene> scene) const {
    // Create game::Stage instance and assign new scene
    game::core::Store::stage = std::make_unique<game::core::Stage>(scene_name, std::move(scene));

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button if defined
    {
        if(this->mouse_)
            this->UpdateMousePosition();

        // Process input and update current active scene
        game::core::Store::stage->Update();



        // Draw
        BeginDrawing();

        ClearBackground(BLACK); // Letterbox color

        // Draw everything in the render texture, note this will not be rendered on screen, yet
        BeginTextureMode(this->render_target_);
        // Draw the current active scene to render texture
        game::core::Store::stage->Draw();
        EndTextureMode();

        // Draw render texture to window, properly scaled
        this->DrawRenderTexture();
        EndDrawing();

        // Update the delta time machine
        if (game::core::Store::dtm != nullptr) // Zeiger soll nicht auf null zeigen
        {
            game::core::Store::dtm->Update();
        }

        // Increment game tick counter
        game::core::Store::ticks++;

    } // Main game loop end
}

Vector2 game::core::Game::ClampValue(Vector2 value, Vector2 MIN, Vector2 MAX) {
    Vector2 result = value;

    result.x = (result.x > MAX.x) ? MAX.x : result.x;
    result.x = (result.x < MIN.x) ? MIN.x : result.x;
    result.y = (result.y > MAX.y) ? MAX.y : result.y;
    result.y = (result.y < MIN.y) ? MIN.y : result.y;

    return result;
}

void game::core::Game::UpdateMousePosition() const {
    // Compute required framebuffer scaling
    float scale = MIN((float) GetScreenWidth() / this->stage_width_, (float) GetScreenHeight() / this->stage_height_);

    // Update virtual mouse (clamped mouse value behind game screen)
    Vector2 mouse = GetMousePosition();
    Store::mouse_position.x = (mouse.x - (static_cast<float>(GetScreenWidth()) - (static_cast<float>(this->stage_width_) * scale)) * 0.5f) / scale;
    Store::mouse_position.y = (mouse.y - (static_cast<float>(GetScreenHeight()) - (static_cast<float>(this->stage_height_) * scale)) * 0.5f) / scale;
    Store::mouse_position = ClampValue(Store::mouse_position, {0, 0}, {static_cast<float>(this->stage_width_), static_cast<float>(this->stage_height_)});
}

void game::core::Game::DrawRenderTexture() const {
    // Compute required framebuffer scaling
    float scale = MIN((float) GetScreenWidth() / this->stage_width_, (float) GetScreenHeight() / this->stage_height_);

    // Draw RenderTexture2D to window, properly scaled
    DrawTexturePro(this->render_target_.texture,
                   {0.0f, 0.0f, static_cast<float>(this->render_target_.texture.width), static_cast<float>(-this->render_target_.texture.height)},
                   {(static_cast<float>(GetScreenWidth()) - (static_cast<float>(this->stage_width_) * scale)) * 0.5f,
                    (static_cast<float>(GetScreenHeight()) - (static_cast<float>(this->stage_height_) * scale)) * 0.5f,
                    static_cast<float>(this->stage_width_) * scale, static_cast<float>(this->stage_height_) * scale},
                   {0.0f, 0.0f}, 0.0f, WHITE
    );
}
// Note: Probably you will never want to make changes directly to this file or
// in the main function! Add your code to the existing scene files under
// src/scenes and/or create new scenes.
// General settings can be made in the config.h.in file.


#include <game.h>
#include "../scenes/scenes.h"
#include "config.h.in"
#include "wall_collisions.h"
#include "DeltaTimeMachine.h"
#include <store.h>

using namespace std::string_literals;

//RenderTexture2D canvas= LoadRenderTexture(940,560);


int main()
{
    DT::timemachine dtm;
    dtm.Start();

    // Weise Adresse dem Zeiger in Store zu
    game::core::Store::dtm = &dtm;

    game::core::Game game(game::Config::kStageWidth, game::Config::kStageHeight, game::Config::kFullScreen,
                          game::Config::kTargetFps, game::Config::kWindowFlags, game::Config::kTextureFilter,
                          game::Config::kExitKey,game::Config::kUseMouse, game::Config::kAudio,
                          game::Config::kProjectName);

    game.Run("menu"s, std::make_unique<game::scenes::MenuScene>());

    return EXIT_SUCCESS;
}
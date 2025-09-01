#pragma once

#include <map>
#include <raylib.h>
#include "Stage.h"
#include "Actor.h"
#include "../game/PlayerClassOne.h"
namespace game::core
{
    struct PlayerState {
        Player_Class_One player;
        int score = 0;
        int souls = 0;

        explicit PlayerState(Vector2 start_pos) : player(start_pos) {}
    };


    struct Store final
    {
        /// The Stage object is responsible for the scene change and for updating and drawing the scene contents
        inline static std::unique_ptr<game::core::Stage> stage = nullptr;

        /// To get the mouse position the mouse support must be enabled when initializing the Game object.
        /// @brief The virtual mouse position.
        inline static Vector2 mouse_Position = { };

        /// Global game counter. Can be used e.g. for the calculation of animation times.
        inline static long long int ticks = 1;

        /// The actors map can contain shared pointers to actor objects that are to be used across
        /// multiple scenes (e.g. the player actor).
        inline static std::map<std::string, std::shared_ptr<game::core::Actor>> actors = { };
        inline static std::unique_ptr<PlayerState> player_state = nullptr;
        inline static int last_score = 0;

    };
}

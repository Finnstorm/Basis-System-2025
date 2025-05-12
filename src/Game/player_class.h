//
// Created by Manza on 4/30/2025.
//

#pragma once

#include "base_class.h"

// Inherits from base class and adds player only logic (like the HP drain from Gauntlet)

class player_base_class : public base_class {
protected:
    float hp_timer = 0.0f;

public:
    void UPDATE(float delta_time) override;
};

// !! not sure if it works correctly !!
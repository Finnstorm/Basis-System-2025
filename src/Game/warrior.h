//
// Created by Manza on 4/30/2025.
//

#pragma once

#include "base_class.h"
#include "config.h"
#include <raylib.h>

class warrior : public base_class {
private:
    Texture2D sprite;

public:
    warrior();
    ~warrior() override;

    void ATTACK() override;
    void UPDATE(float delta_time) override;
    void DRAW() const;
};

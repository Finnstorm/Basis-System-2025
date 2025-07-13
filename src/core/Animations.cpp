//
// Created by Kruse on 02/07/2025.
//


#include "Animations.h"

Animations::Animations(Vector2 sprite_size, const char* filename, int FC, int spl, float speed) {
    this->spritesheet = LoadTexture(filename);
    this->size = sprite_size;
    this->frame_Count = FC;
    this->sprites_per_line = spl;
    this->target = {1, 1, this->size.x, this->size.y};

    // NEU: Zeit pro Frame berechnen
    this->time_per_frame = speed / 10.0f;
}

void Animations::First_Frame() {
    target.x = 1;
    target.y = 1;
    this->current_Frame = 0;
    this->time_accumulator = 0.0f;
}

void Animations::Update_Frame(float delta_time) {
    if (this->current_Frame >= this->frame_Count - 1) {
        return;
    }

    this->time_accumulator += delta_time;
    if (this->time_accumulator >= this->time_per_frame) {
        this->time_accumulator -= this->time_per_frame;
        this->current_Frame++;

        if (this->current_Frame % this->sprites_per_line == 0 && current_Frame != 0) {
            this->target.x = 1;
            this->target.y += this->size.y;
        } else {
            this->target.x += this->size.x;
        }
    }
}

void Animations::Draw_Current_Frame(Vector2 pos) {
    DrawTextureRec(spritesheet, target, pos, WHITE);
}
//
// Created by Kruse on 02/07/2025.
//


#include "Animations.h"

Animations::Animations(Vector2 sprite_size, const char* filename, int FC, int spl, float speed)
{
    if (filename != nullptr)
    {
        this->spritesheet = LoadTexture(filename);
    }
    this->size = sprite_size;
    this->frame_Count = FC;
    this->sprites_per_line = spl > 0 ? spl : 1;
    this->target = {0, 0, this->size.x, this->size.y};

    if (speed > 0.0f) {
        this->time_per_frame = 1.0f / speed;
    } else {
        this->time_per_frame = 0.0f;
    }
}

Animations::Animations(Vector2 sprite_size, Texture2D spritesheet, int FC, int spl, float speed)
{
    this->spritesheet = spritesheet; // Direkt die geladene Textur zuweisen
    this->size = sprite_size;
    this->frame_Count = FC;
    this->sprites_per_line = spl > 0 ? spl : 1;
    this->target = {0, 0, this->size.x, this->size.y};

    if (speed > 0.0f) {
        this->time_per_frame = 1.0f / speed;
    } else {
        this->time_per_frame = 0.0f;
    }
}

void Animations::First_Frame()
{
    target.x = 0;
    target.y = 0;
    this->current_Frame = 0;
    this->time_accumulator = 0.0f;
}

void Animations::Update_Frame(float delta_time)
{
    this->time_accumulator += delta_time;
    if (this->time_accumulator >= this->time_per_frame)
        {
        if (this->current_Frame < this->frame_Count - 1)
        {
            this->time_accumulator -= this->time_per_frame;
            this->current_Frame++;
            int row = this->current_Frame / this->sprites_per_line;
            int col = this->current_Frame % this->sprites_per_line;

            this->target.x = col * this->size.x;
            this->target.y = row * this->size.y;
        }
        else
        {
            this->time_accumulator = 0.0f;
        }
    }
}

void Animations::Draw_Current_Frame(Vector2 pos)
{
    if (spritesheet.id <= 0) return;
    DrawTextureRec(spritesheet, target, pos, WHITE);
}

bool Animations::IsFinished() const
{
    return this->current_Frame >= this->frame_Count - 1;
}
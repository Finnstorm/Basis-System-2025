#include "RepeatAnimation.h"

RepeatAnimation::RepeatAnimation(Vector2 sprite_size, const char* filename, int FC, int spl, float speed)
{
    if (speed > 0.0f) {
        this->time_per_frame = 1.0f / speed;
    } else {
        this->time_per_frame = 0.0f;
    }

    if (filename != nullptr) {
        this->spritesheet = LoadTexture(filename);
    }

    this->size = sprite_size;
    this->frame_Count = FC;
    this->sprites_per_line = spl > 0 ? spl : 1;
    this->target = { 0, 0, this->size.x, this->size.y };
}

RepeatAnimation::RepeatAnimation(Vector2 sprite_size, Texture2D spritesheet, int FC, int spl, float speed)
{
    if (speed > 0.0f) {
        this->time_per_frame = 1.0f / speed;
    } else {
        this->time_per_frame = 0.0f;
    }

    this->spritesheet = spritesheet;

    this->size = sprite_size;
    this->frame_Count = FC;
    this->sprites_per_line = spl > 0 ? spl : 1;
    this->target = { 0, 0, this->size.x, this->size.y };
}

void RepeatAnimation::First_Frame()
{
    this->current_Frame = 0;
    this->time_accumulator = 0.0f;
    this->target.x = 0;
    this->target.y = 0;
}

void RepeatAnimation::Update_Frame(float delta_time)
{
    if (time_per_frame <= 0.0f || frame_Count == 0) return;

    this->time_accumulator += delta_time;

    if (this->time_accumulator >= this->time_per_frame)
    {
        this->time_accumulator -= this->time_per_frame;
        this->current_Frame++;

        if (this->current_Frame >= this->frame_Count) {
            First_Frame();
        }

        int row = this->current_Frame / this->sprites_per_line;
        int col = this->current_Frame % this->sprites_per_line;

        this->target.x = col * this->size.x;
        this->target.y = row * this->size.y;
    }
}

void RepeatAnimation::Draw_Current_Frame(Vector2 pos) {
    if (spritesheet.id <= 0) return;
    DrawTextureRec(spritesheet, target, pos, WHITE);
}

void RepeatAnimation::Draw_Current_Frame_Pro(Vector2 pos) {
    if (spritesheet.id <= 0) return;

    // draw current frame (target) at pos, scaled x2, no rotation
    const Rectangle dest{ pos.x, pos.y, target.width * 4.0f, target.height * 4.0f };
    const Vector2   origin{ 0.0f, 0.0f };

    DrawTexturePro(spritesheet, target, dest, origin, 0.0f, WHITE);
}
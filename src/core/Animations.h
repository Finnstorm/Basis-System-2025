//
// Created by Kruse on 02/07/2025.
//

#ifndef RAYLIBSTARTER_ANIMATIONS_H
#define RAYLIBSTARTER_ANIMATIONS_H

#include <raylib.h>
#include <memory>

class Animations {
private:
    int current_Frame = 0;
    int frame_Count;
    int sprites_per_line;

    Rectangle target;
    Texture2D spritesheet;

    float time_accumulator = 0.0f;
    float time_per_frame;

public:
    Animations(Vector2 sprite_size, const char* filename, int FC, int spl, float speed);
    Animations(Vector2 sprite_size, Texture2D spritesheet, int FC, int spl, float speed);
    Vector2 size;
    void Update_Frame(float delta_time);
    void First_Frame();
    void Draw_Current_Frame(Vector2);
    bool IsFinished() const;
    int GetCurrentFrame() const { return current_Frame; }
    Texture2D GetSpritesheet() const { return spritesheet; }
    Rectangle GetCurrentFrameRec() const { return target; }
};

#endif //RAYLIBSTARTER_ANIMATIONS_H

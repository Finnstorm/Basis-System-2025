//
// Created by Kruse on 07/07/2025.
//

#ifndef RAYLIBSTARTER_REPEATANIMATION_H
#define RAYLIBSTARTER_REPEATANIMATION_H

#include <raylib.h>
#include <memory>
class RepeatAnimation {

    private:
        int current_Frame = 0;
        float time_accumulator = 0.0f;
        float time_per_frame;
        int frame_Count;
        int sprites_per_line;

        Rectangle target;
        Texture2D spritesheet;
    public:
    Vector2 size;
    RepeatAnimation(Vector2, const char*, int, int, float speed);
    RepeatAnimation(Vector2, Texture2D, int, int, float speed);
    void Update_Frame(float delta_time);
    void First_Frame();
    void Draw_Current_Frame(Vector2);
    void Draw_Current_Frame_Pro(Vector2);

};


#endif //RAYLIBSTARTER_REPEATANIMATION_H

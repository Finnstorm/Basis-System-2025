#include "RepeatAnimation.h"

RepeatAnimation::RepeatAnimation(Vector2 sprite_size, const char* filename, int FC, int spl, float speed)
{
    // Korrekte Geschwindigkeitsberechnung (Frames pro Sekunde)
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
    this->sprites_per_line = spl > 0 ? spl : 1; // Verhindert Division durch Null
    this->target = { 0, 0, this->size.x, this->size.y };
}

// Die fehlende Funktionsdefinition
void RepeatAnimation::First_Frame() {
    this->current_Frame = 0;
    this->time_accumulator = 0.0f; // Setzt auch den Zeit-Akkumulator zurück

    // Setzt die Ziel-Rectangle auf den ersten Frame (0,0)
    this->target.x = 0;
    this->target.y = 0;
}

void RepeatAnimation::Update_Frame(float delta_time)
{
    // Nichts tun, wenn keine Geschwindigkeit gesetzt ist
    if (time_per_frame <= 0.0f || frame_Count == 0) return;

    this->time_accumulator += delta_time;

    if (this->time_accumulator >= this->time_per_frame)
    {
        this->time_accumulator -= this->time_per_frame;
        this->current_Frame++;

        // Wenn die Animation durchgelaufen ist, fange von vorne an.
        if (this->current_Frame >= this->frame_Count) {
            First_Frame(); // Hier war der Aufruf, der die Definition benötigt.
        }

        // Berechne die Position des Frames im Spritesheet neu.
        // Das ist robuster als die vorherige Logik.
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
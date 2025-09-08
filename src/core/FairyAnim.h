#ifndef FAIRYANIM_H
#define FAIRYANIM_H


struct FairyAnimMeta {
    Vector2 frameSize;   // size of one frame in the sheet
    int     frames;      // total frames in the sheet
    int     columns;     // frames per row (sprites_per_line)
    float   fps;         // playback speed
};

// Adjust these numbers to your actual sheets:
inline FairyAnimMeta GetFairyMetaForLevel(int lvl) {
    switch (lvl) {
        case 1: return FairyAnimMeta{ Vector2{145.0f, 176.0f}, 11, 11, 8.0f };
        case 2: return FairyAnimMeta{ Vector2{145.0f, 176.0f}, 11, 11, 8.0f };
        case 3: return FairyAnimMeta{ Vector2{145.0f, 176.0f}, 11, 11, 8.0f };
        default: return FairyAnimMeta{ Vector2{145.0f, 176.0f},  11, 11, 1.0f };
    }
}

#endif //FAIRYANIM_H

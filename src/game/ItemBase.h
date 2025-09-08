//
// Created by $Will on 27.08.2025.
//

#ifndef ITEMBASE_H
#define ITEMBASE_H

#include "Collidable.h"
#include "../core/RepeatAnimation.h"
class Player_Base_Class;

enum class ItemType
{
    HEALTH_POTION,
    BOMB,
    TESTO_NEEDLE,
    KEY,
    FAIRY
};

class ItemBase : public Collidable
{
protected:
    ItemType type;
    RepeatAnimation animation;
    Rectangle attraction_hitbox;

    float despawn_timer;
    float blink_timer;
    bool is_being_attracted = false;
    bool has_despawn_timer;
    bool is_rendered = true;

public:
    ItemBase(Vector2 position, ItemType type, const char* anim_path, bool has_timer,
         Vector2 anim_size, int frame_count, float anim_speed);
    virtual ~ItemBase() = default;
    void Tick(float delta_time) override;
    void On_Collision(Collidable* other) override;
    void Draw() override;
    virtual void Activate(Player_Base_Class* player) = 0;

    ItemType GetType() const { return type; }
    Rectangle GetAttractionHitbox() const { return attraction_hitbox; }
    bool IsBeingAttracted() const { return is_being_attracted; }
    void SetAttracted(bool attracted) { is_being_attracted = attracted; }
    Collision_Type Get_Collision_Type() const override { return Collision_Type::CONSUMABLE; }
    void Set_Position(Vector2 position) override;
};


#endif //ITEMBASE_H

//
// Created by $Will on 27.08.2025.
//

#include "ItemBase.h"
#include "PlayerBaseClass.h"
#include "../Config.h.in"
#include "raymath.h"

ItemBase::ItemBase(Vector2 position, ItemType item_type, const char* anim_path, bool has_timer,
                   Vector2 anim_size, int frame_count, float anim_speed)
    : type(item_type),
      animation(anim_size, anim_path, frame_count, frame_count, anim_speed),
      has_despawn_timer(has_timer)
{
    this->hitbox =
    {
        position.x,
        position.y,
        game::Config::item_Pickup_Hitbox_Size.x,
        game::Config::item_Pickup_Hitbox_Size.y
    };

    this->attraction_hitbox = {
        position.x - (game::Config::item_Attraction_Hitbox_Size.x - hitbox.width) / 2,
        position.y - (game::Config::item_Attraction_Hitbox_Size.y - hitbox.height) / 2,
        game::Config::item_Attraction_Hitbox_Size.x,
        game::Config::item_Attraction_Hitbox_Size.y
    };

    if (has_despawn_timer)
    {
        this->despawn_timer = game::Config::item_Despawn_Time;
    }
    else
    {
        this->despawn_timer = -1.0f;
    }
    this->blink_timer = 0.0f;
}

void ItemBase::Tick(float delta_time)
{
    animation.Update_Frame(delta_time);
    if (has_despawn_timer)
    {
        despawn_timer -= delta_time;

        if (despawn_timer <= 0.0f)
        {
            Mark_For_Destruction();
            return;
        }

        if (despawn_timer <= game::Config::item_Blinking_Start_Time)
        {
            blink_timer += delta_time;
            if (blink_timer >= game::Config::item_Blink_Interval)
            {
                is_rendered = !is_rendered;
                blink_timer = 0.0f;
            }
        }
    }

    if (is_being_attracted) { }
    this->attraction_hitbox.x = this->hitbox.x - (game::Config::item_Attraction_Hitbox_Size.x - hitbox.width) / 2;
    this->attraction_hitbox.y = this->hitbox.y - (game::Config::item_Attraction_Hitbox_Size.y - hitbox.height) / 2;
}

void ItemBase::On_Collision(Collidable* other) { }

void ItemBase::Draw()
{
    if (is_rendered)
    {
        animation.Draw_Current_Frame({hitbox.x, hitbox.y});
    }
}
void ItemBase::Set_Position(Vector2 position)
{
    this->hitbox.x = position.x;
    this->hitbox.y = position.y;
}

//
// Created by $Will on 22.06.2025.
//

#ifndef PLAYERCLASSONE_H
#define PLAYERCLASSONE_H


#include <map>
#include "PlayerBaseClass.h"
#include "Animations.h"
#include "RepeatAnimation.h"
#include "../Config.h.in"

class Player_Class_One : public Player_Base_Class
{
private:
    Vector2 size = game::Config::player_Hittbox;
    std::map<Facing_Direction, RepeatAnimation> walking_Animations;
    std::map<Facing_Direction, RepeatAnimation> idle_Animations;
    std::map<Facing_Direction, Animations> ranged_Attack_Animations;
    std::map<Facing_Direction, Animations> melee_Attack_Animations;
    std::map<Facing_Direction, RepeatAnimation> buff_walking_Animations;
    std::map<Facing_Direction, RepeatAnimation> buff_idle_Animations;
    std::map<Facing_Direction, Animations> buff_melee_Attack_Animations;
    Facing_Direction attack_Direction = DOWN;
public:
    PlayerState previousState = IDLE;
    Animations test_animation{size,"assets/graphics/anim_sprite.png",9,3,3};
    Player_Class_One(Vector2 start_Position);
    ~Player_Class_One();
    void Draw() override;
    void Tick(float delta_time) override;
    void Ranged_Attack() override;
    void Melee_Attack() override;
    void ReapplyUpgrades();
};


#endif //PLAYERCLASSONE_H

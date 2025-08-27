//
// Created by Kruse on 23/05/2025.
//
#pragma once
#include "ItemBase.h"
#include <list>
#include "raylib.h"
#include "Collidable.h"
#include "CollisionManager.h"
namespace game { class Player_Projectile; }

class Object_Manager;
class Collision_Manager;

enum Facing_Direction {UP, DOWN, LEFT, RIGHT, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT, NONE};
enum PlayerState {IDLE, WALKING, ATTACKING_RANGED, ATTACKING_MELEE};
enum class Input_Direction { UP, DOWN, LEFT, RIGHT };

class Player_Base_Class : public Collidable
{
protected:
	Vector2 current_Sprite_Size;
	PlayerState currentState = IDLE;
	float player_Health;
	int player_Max_Health;
	float player_Movement_Speed;
	float player_Damage_Multiplier;

	ItemBase* held_item = nullptr;
	bool is_buffed = false;
	float buff_timer = 0.0f;
	float original_movement_speed;
	float original_damage_multiplier;
	bool has_fairy = false;

    Vector2 player_Pos;
	Vector2 previous_Position;
	Collision_Manager* manager_Ptr;
	float range_Attack_Cooldown;
	float melee_Cooldown;
	bool inventory_Is_Full;
	Facing_Direction facing_Direction = DOWN;
	bool is_Moving;
	std::list<Input_Direction> horizontal_inputs;
	std::list<Input_Direction> vertical_inputs;

    Texture2D maintex= LoadTexture("assets/graphics/ball.png");

    float projectile_Speed;
    std::vector<std::unique_ptr<game::Player_Projectile>> sp_projectiles;

public:

	Player_Base_Class(int max_Health, float movement_Speed, float damage_multiplier, Vector2 start_Position);
	bool Is_Dead() const;

	~Player_Base_Class() override;
	void Player_Input();
	void Tick(float delta_time) override;
	void On_Collision(Collidable* other) override;
	virtual void Draw() override;

	virtual void Melee_Attack();

	void Update_Previous_Position();
	void Update_Facing_Direction();
	void Update_Input_Stacks();
	virtual void Ranged_Attack();
	float Get_Health() const;
    Collision_Type Get_Collision_Type() const override;
    Vector2 Get_Player_Pos();
    Vector2 Get_Player_Center();
	void Set_Position(Vector2 position) override;
    void Take_Damage(int damage);

	Object_Manager* object_manager_ptr = nullptr;

	void Use_Item();
	void PickUpItem(ItemBase* item_to_pick_up);
	bool HasItem() const;
	ItemBase* GetHeldItem() const { return held_item; }
	void RemoveHeldItem();
	void ApplyTestoBuff();
	bool IsBuffed() const;
	void SetHasFairy(bool value) { has_fairy = value; }
	bool HasFairy() const { return has_fairy; }
};


//
// Created by $Will on 01.06.2025.
//

#ifndef WALL_COLLISIONS_H
#define WALL_COLLISIONS_H

#include <iostream>

#include "raylib.h"
#include <string>
#include <vector>
#include "Playerinput.h"
#include "../config.Puzzle1.h.in"

namespace game::core {

    class Wall {
    protected:
        Rectangle rect_;
        std::string type_;
        Vector2 position_;

    public:
        // Konstruktoren bleiben im Header, da sie primär Initialisierungslisten verwenden oder einfach sind.
        Wall(Rectangle rectangle, const std::string& wall_type)
            : rect_(rectangle), type_(wall_type), position_({ rectangle.x, rectangle.y }) {}

        Wall(Vector2 top_left_position, float width, float height, const std::string& wall_type)
            : rect_({top_left_position.x, top_left_position.y, width, height}),
              type_(wall_type),
              position_(top_left_position) {}

        virtual ~Wall() = default; // Default-Destruktor kann im Header bleiben

        // Getter können inline im Header implementiert werden
        virtual Rectangle Get_Rect() const { return rect_; }
        std::string Get_Type() const { return type_; }
        Vector2 Get_Position() const { return position_; }

        // Update und Draw werden nur DEKLARIERT, Implementierung in wall.cpp
        virtual void Update(float delta_time, const std::vector<Wall*>& all_walls);
        virtual void Draw() const;
    };

    class Movable_Wall : public Wall
    {
    private:
        Vector2 target_position_pixel_;
        bool is_solved_ = false;
        bool is_moving_ = false;
        Vector2 sprite_render_destination_on_tile_;
        float current_push_delay_timer_ = 0.0f;
        float current_move_cooldown_timer_ = 0.0f;

        const char* path_sprite_inactive_top_;
        const char* path_sprite_inactive_bottom_;
        const char* path_sprite_active_top_;
        const char* path_sprite_active_bottom_;

        // Private Hilfsmethode wird nur DEKLARIERT, Implementierung in movable_wall.cpp
        void Update_Hitbox_Rectangle();

    public:
        // Konstruktor wird nur DEKLARIERT, Implementierung in movable_wall.cpp
        Movable_Wall(Vector2 initial_top_left_pos,
                     Vector2 target_pixel_pos,
                     const char* inactive_top_sprite_path,
                     const char* inactive_bottom_sprite_path,
                     const char* active_top_sprite_path,
                     const char* active_bottom_sprite_path);
        float DEBUG_Get_Push_Timer() const { return current_push_delay_timer_; }
        float DEBUG_Get_Cooldown_Timer() const { return current_move_cooldown_timer_; }
        void Start_Push_Attempt()
        {

            if (!is_moving_ && !is_solved_ && current_move_cooldown_timer_ <= 0.0f)
            {
            // Starte den Push-Delay-Timer NUR, wenn er nicht schon aktiv ist (oder gerade erst abgelaufen ist)
            // Eine einfache Möglichkeit ist, ihn nur zu setzen, wenn er <= 0 ist.
            // Wenn er bereits läuft ( > 0), lassen wir ihn weiterlaufen.
            if (current_push_delay_timer_ <= 0.0f)
                {
                current_push_delay_timer_ = puzzle1::Config_Puzzle1::movable_Wall_Push_Delay;
                std::cout << "Movable_Wall: Push attempt BEGINNING, PushDelayTimer set to " << current_push_delay_timer_ << std::endl;
                }
                else
                    {
                        // Optional: Debug, dass der Timer schon läuft
                        // std::cout << "Movable_Wall: Push attempt ONGOING,
                        // PushDelayTimer is " << current_push_delay_timer_ << std::endl;
                    }
            }
            else
                {
                // std::cout << "Movable_Wall: Start_Push_Attempt called, but wall not ready (moving=" << is_moving_
                // << ", solved=" << is_solved_ << ", cooldown=" << current_move_cooldown_timer_ << ")" << std::endl;
                }
}

        bool Is_Ready_To_Move() const { // Bleibt inline
            return current_push_delay_timer_ <= 0.0f && !is_moving_ && !is_solved_ && current_move_cooldown_timer_ <= 0.0f;
        }

        bool Is_Solved() const { return is_solved_; } // Bleibt inline
        bool Is_Currently_Moving() const { return is_moving_; } // Bleibt inline

        // Komplexere Methoden werden nur DEKLARIERT, Implementierung in movable_wall.cpp
        void Attempt_Move(Player::Direction push_direction, const std::vector<Wall*>& all_walls);
        void Update(float delta_time, const std::vector<Wall*>& all_walls) override; // Überschreibt Wall::Update
        void Check_If_Solved();
        void Draw() const override; // Überschreibt Wall::Draw
    };

} // namespace game::core
#endif //WALL_COLLISIONS_H

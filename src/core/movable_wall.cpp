//
// Created by $Will on 01.06.2025.
//
#include "wall_collisions.h"      // Enthält die Deklaration von Movable_Wall
#include "collision_check.h" // Für game::core::check_collision
#include "../config.h.in"         // Für game::Config::tile_Size
#include "../config.Puzzle1.h.in"" // Für puzzle1::Config_Puzzle1::... (wird zu config.h.Puzzle1)
#include <iostream>               // Für std::cout (Debug)
#include <cmath>                  // Für fabs
#include <raymath.h>              // Für Vector2Length, Vector2Normalize (falls nicht schon durch raylib.h abgedeckt)

namespace game::core {

// Implementierung für den Movable_Wall Konstruktor
Movable_Wall::Movable_Wall(Vector2 initial_top_left_pos,
                             Vector2 target_pixel_pos,
                             const char* inactive_top_sprite_path,
                             const char* inactive_bottom_sprite_path,
                             const char* active_top_sprite_path,
                             const char* active_bottom_sprite_path)
    : Wall(initial_top_left_pos, // Basis-Konstruktor für position_
           puzzle1::Config_Puzzle1::movable_Wall_Hitbox_Width,    // Breite der Hitbox aus Puzzle1-Config
           puzzle1::Config_Puzzle1::movable_Wall_Hitbox_Height,   // Höhe der Hitbox aus Puzzle1-Config
           "Movable_Wall"),                                      // Typ-String
      target_position_pixel_(target_pixel_pos),
      // is_solved_ und is_moving_ werden durch Member-Initialisierung in der .h auf false gesetzt
      sprite_render_destination_on_tile_(initial_top_left_pos),
      // current_push_delay_timer_ und current_move_cooldown_timer_ werden durch Member-Initialisierung in .h auf 0.0f gesetzt
      path_sprite_inactive_top_(inactive_top_sprite_path),
      path_sprite_inactive_bottom_(inactive_bottom_sprite_path),
      path_sprite_active_top_(active_top_sprite_path),
      path_sprite_active_bottom_(active_bottom_sprite_path) {

    // Setze die Hitbox-Position korrekt basierend auf der visuellen Position und den Offsets/Dimensionen aus der Config.
    Update_Hitbox_Rectangle();
}

// Implementierung für die private Hilfsmethode
void Movable_Wall::Update_Hitbox_Rectangle() {
    // position_ (aus Basisklasse Wall) ist die obere linke Ecke des visuellen 2x2 Sprites.
    // rect_ (aus Basisklasse Wall) ist die Hitbox.
    rect_.x = position_.x + puzzle1::Config_Puzzle1::movable_Wall_Hitbox_Offset_X; // Korrekter Config-Pfad
    rect_.y = position_.y + puzzle1::Config_Puzzle1::movable_Wall_Hitbox_Offset_Y; // Korrekter Config-Pfad
    // rect_.width und rect_.height wurden bereits im Wall-Basisklassenkonstruktor gesetzt
    // mit puzzle1::Config_Puzzle1::movable_Wall_Hitbox_Width und ::movable_Wall_Hitbox_Height.
}

// Implementierung für Attempt_Move
void Movable_Wall::Attempt_Move(Player::Direction push_direction, const std::vector<Wall*>& all_walls) {
    if (is_solved_ || is_moving_ || current_move_cooldown_timer_ > 0.0f) {
        return;
    }

    Vector2 move_offset_pixels = {0.0f, 0.0f};
    float tile_s = game::Config::tile_Size;

    switch (push_direction) {
        case Player::Up:    move_offset_pixels.y = -tile_s; break;
        case Player::Down:  move_offset_pixels.y =  tile_s; break;
        case Player::Left:  move_offset_pixels.x = -tile_s; break;
        case Player::Right: move_offset_pixels.x =  tile_s; break;
        default: return;
    }

    Vector2 next_visual_top_left = {position_.x + move_offset_pixels.x, position_.y + move_offset_pixels.y};

    Rectangle next_step_hitbox = {
        next_visual_top_left.x + puzzle1::Config_Puzzle1::movable_Wall_Hitbox_Offset_X,
        next_visual_top_left.y + puzzle1::Config_Puzzle1::movable_Wall_Hitbox_Offset_Y,
        puzzle1::Config_Puzzle1::movable_Wall_Hitbox_Width,
        puzzle1::Config_Puzzle1::movable_Wall_Hitbox_Height
    };

    bool collision_at_next_step = false;
    for (const auto* other_wall : all_walls) {
        if (other_wall == this) continue;
        if (game::core::check_collision(next_step_hitbox, other_wall->Get_Rect())) {
            collision_at_next_step = true;
            std::cout << "Movable_Wall: Path blocked at (" << next_visual_top_left.x << "," << next_visual_top_left.y << ") by another wall." << std::endl;
            break;
        }
    }
    // TODO: Kollision mit Spieler und Gegnern prüfen

    if (!collision_at_next_step) {
        is_moving_ = true;
        sprite_render_destination_on_tile_ = next_visual_top_left;
        std::cout << "Movable_Wall: Movement initiated towards (" << next_visual_top_left.x << ", " << next_visual_top_left.y << ")" << std::endl;
    } else {
        current_push_delay_timer_ = 0.0f;
    }
}

// Implementierung für Movable_Wall::Update
void Movable_Wall::Update(float delta_time, const std::vector<Wall*>& all_walls) {
    if (is_solved_) return;

    if (current_move_cooldown_timer_ > 0.0f) {
        current_move_cooldown_timer_ -= delta_time;
        if (current_move_cooldown_timer_ < 0.0f) current_move_cooldown_timer_ = 0.0f;
    }

    if (current_push_delay_timer_ > 0.0f) {
        current_push_delay_timer_ -= delta_time;
        if (current_push_delay_timer_ < 0.0f) current_push_delay_timer_ = 0.0f;
    }

    if (is_moving_) {
        Vector2 direction_to_target = {
            sprite_render_destination_on_tile_.x - position_.x,
            sprite_render_destination_on_tile_.y - position_.y
        };
        float distance_to_target = Vector2Length(direction_to_target);
        float move_step_pixels = puzzle1::Config_Puzzle1::movable_Wall_Sprite_Movement_Speed * delta_time;

        if (distance_to_target <= move_step_pixels) {
            position_ = sprite_render_destination_on_tile_;
            Update_Hitbox_Rectangle();
            is_moving_ = false;
            current_move_cooldown_timer_ = puzzle1::Config_Puzzle1::movable_Wall_Move_Cooldown;
            std::cout << "Movable_Wall: Movement finished. Position: (" << position_.x << ", " << position_.y << ")" << std::endl;
            Check_If_Solved();
        } else {
            Vector2 normalized_dir = Vector2Normalize(direction_to_target);
            position_.x += normalized_dir.x * move_step_pixels;
            position_.y += normalized_dir.y * move_step_pixels;
            Update_Hitbox_Rectangle();
        }
    }
}

// Implementierung für Check_If_Solved
void Movable_Wall::Check_If_Solved() {
    if (is_solved_) return;

    float epsilon = 0.1f;
    if (fabs(position_.x - target_position_pixel_.x) < epsilon &&
        fabs(position_.y - target_position_pixel_.y) < epsilon) {
        is_solved_ = true;
        std::cout << "Movable_Wall at (" << position_.x << ", " << position_.y << ") IS SOLVED!" << std::endl;
    }
}

// Implementierung für Movable_Wall::Draw
void Movable_Wall::Draw() const {
    Color current_color = DARKBLUE;
    if (is_solved_) {
        current_color = GREEN;
    } else if (is_moving_) {
        current_color = YELLOW;
    }

    // Zeichne die visuelle Repräsentation (2x2 Kacheln)
    // position_ (aus Basisklasse) ist die obere linke Ecke des visuellen Sprites.
    DrawRectangleV(position_,
                   {puzzle1::Config_Puzzle1::movable_Wall_Visual_Width, puzzle1::Config_Puzzle1::movable_Wall_Visual_Height},
                   current_color);

    // Debug-Hitbox zeichnen (optional)
    // DrawRectangleLinesEx(rect_, 2, VIOLET);
}

} // namespace game::core
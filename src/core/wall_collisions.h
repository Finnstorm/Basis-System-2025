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
#include "../config.h.in"

namespace game::core {

    // Basisklasse für alle Wandtypen
    class Wall {
    protected:
        // Die Kollisions-Hitbox der Wand.
        // Für statische Wände ist das einfach das Rechteck aus Tiled.
        // Für dynamische Wände (wie Movable_Wall) wird dies basierend auf ihrer aktuellen Position und Config-Offsets/Dimensionen berechnet.
        Rectangle rect_;

        // Der Typ der Wand (z.B. "Wall", "Movable_Wall"), aus Tiled oder intern gesetzt.
        std::string type_;

        // Die logische/visuelle Top-Left-Position des Objekts.
        // Für einfache Wände ist das rect_.x, rect_.y.
        // Für Movable_Wall ist das die Top-Left-Position des 2x2 visuellen Sprites.
        Vector2 position_;

    public:
        // Konstruktor für Wände, deren Hitbox direkt durch ein Rechteck aus Tiled definiert wird.
        Wall(Rectangle rectangle, const std::string& wall_type)
            : rect_(rectangle), type_(wall_type), position_({ rectangle.x, rectangle.y }) {}

        // Konstruktor für Wände, deren Position und Dimensionen explizit übergeben werden
        // (nützlich für Movable_Wall, wo die Hitbox-Dimensionen aus der Config kommen).
        Wall(Vector2 top_left_position, float width, float height, const std::string& wall_type)
            : rect_({top_left_position.x, top_left_position.y, width, height}),
              type_(wall_type),
              position_(top_left_position) {}

        virtual ~Wall() = default;

        // Gibt die Kollisions-Hitbox der Wand zurück.
        // Kann von abgeleiteten Klassen überschrieben werden, wenn die Hitbox dynamisch ist.
        virtual Rectangle Get_Rect() const {
            return rect_;
        }

        std::string Get_Type() const {
            return type_;
        }

        // Gibt die logische/visuelle Top-Left-Position des Objekts zurück.
        Vector2 Get_Position() const {
            return position_;
        }

        // Basis-Update, kann von abgeleiteten Klassen für spezifische Logik überschrieben werden.
        virtual void Update(float delta_time, const std::vector<Wall*>& all_walls) {
            // Standard-Wand macht nichts im Update
        }

        // Basis-Draw, kann von abgeleiteten Klassen für spezifische Darstellung überschrieben werden.
        // Momentan nur für Debug-Zwecke, da Kacheln die Wände darstellen.
        virtual void Draw() const {
            // DrawRectangleLinesEx(rect_, 1, GRAY); // Optionale Debug-Anzeige der Hitbox
        }
    };

    // Klasse für bewegliche Wände (erbt von Wall)
    // Diese Klasse wird EINE spezifische bewegliche Wand repräsentieren (z.B. Movable_Wall_1)
    // Später könnten wir eine allgemeinere MovableWall-Klasse machen, die Daten von außen bekommt.
    class Movable_Wall : public Wall {
    private:
        Vector2 target_position_pixel_; // Zielposition (obere linke Ecke der 2x2 visuellen Repräsentation)

        bool is_solved_;      // Ist die Wand an ihrer Zielposition und das Rätsel dafür gelöst?
        bool is_moving_;      // Bewegt sich die Wand gerade visuell?

        // current_top_left_visual_position_ wird durch position_ der Basisklasse Wall repräsentiert.
        // Wir verwenden position_ für die aktuelle obere linke Ecke des 2x2 Sprites.

        Vector2 sprite_render_destination_on_tile_; // Obere linke Ecke der Kachel, zu der sich das Sprite bewegt.

        float current_push_delay_timer_;    // Timer für die Verzögerung, bevor sich die Wand bewegt.
        float current_move_cooldown_timer_; // Timer für die Abklingzeit nach einer Bewegung.

        // Sprite-Pfade (werden im Konstruktor gesetzt)
        // Für die spätere Implementierung mit geteilten Sprites (Top/Bottom)
        const char* path_sprite_inactive_top_;
        const char* path_sprite_inactive_bottom_;
        const char* path_sprite_active_top_;
        const char* path_sprite_active_bottom_;

        // Aktualisiert die rect_ (Hitbox) basierend auf der aktuellen position_ (visuelle Top-Left)
        // und den Hitbox-Offsets/-Dimensionen aus der Config.
        void Update_Hitbox_Rectangle() {
            rect_.x = position_.x + game::Config::movable_Wall_Hitbox_Offset_X;
            rect_.y = position_.y + game::Config::movable_Wall_Hitbox_Offset_Y;
            // Breite und Höhe der Hitbox sind bereits im Konstruktor über die Basisklasse gesetzt worden.
        }

    public:
        Movable_Wall(Vector2 initial_top_left_pos, // Startposition (obere linke Ecke des 2x2 visuellen Sprites) aus Tiled
                     Vector2 target_pixel_pos,     // Zielposition (obere linke Ecke) für diese spezifische Wand
                     // Sprite-Pfade für diese spezifische Wand:
                     const char* inactive_top_sprite_path,
                     const char* inactive_bottom_sprite_path,
                     const char* active_top_sprite_path,
                     const char* active_bottom_sprite_path)
            : Wall(initial_top_left_pos, // Wird für position_ in der Basisklasse verwendet
                   game::Config::Puzzle1::movable_Wall_Hitbox_Width,
                   game::Config::Puzzle1::movable_Wall_Hitbox_Height,
                   "Movable_Wall"), // Ruft den spezifischeren Konstruktor der Basisklasse auf
              target_position_pixel_(target_pixel_pos),
              is_solved_(false),
              is_moving_(false),
              sprite_render_destination_on_tile_(initial_top_left_pos),
              current_push_delay_timer_(0.0f),
              current_move_cooldown_timer_(0.0f),
              path_sprite_inactive_top_(inactive_top_sprite_path),
              path_sprite_inactive_bottom_(inactive_bottom_sprite_path),
              path_sprite_active_top_(active_top_sprite_path),
              path_sprite_active_bottom_(active_bottom_sprite_path) {

            // Nachdem die Basisklasse Wall initialisiert wurde (inkl. rect_ mit Config-Dimensionen),
            // müssen wir die X/Y-Position der Hitbox (rect_) korrekt setzen,
            // basierend auf dem Offset von der visuellen Position (position_).
            Update_Hitbox_Rectangle();
        }

        // Wird aufgerufen, wenn der Spieler versucht, die Wand zu schieben (Push-Delay startet).
        void Start_Push_Attempt() {
            if (!is_moving_ && !is_solved_ && current_move_cooldown_timer_ <= 0.0f && current_push_delay_timer_ <= 0.0f) {
                current_push_delay_timer_ = game::Config::Puzzle1::movable_Wall_Push_Delay;
                std::cout << "Movable_Wall: Push attempt started, delay timer set." << std::endl;
            }
        }

        // Wird vom Spieler aufgerufen, wenn der Push-Delay abgelaufen ist und der Spieler immer noch schiebt.
        // `all_walls` enthält Zeiger auf alle anderen Wände (statisch und beweglich) zur Kollisionsprüfung.
        void Try_To_Move(Player::Direction push_direction, const std::vector<Wall*>& all_walls) {
            if (is_solved_ || is_moving_ || current_move_cooldown_timer_ > 0.0f) {
                return; // Kann nicht bewegt werden, wenn schon gelöst, in Bewegung oder im Cooldown.
            }
            // Der Push-Delay muss hier nicht erneut geprüft werden, das macht die aufrufende Logik (Spieler).

            Vector2 move_offset_pixels = {0.0f, 0.0f};
            float tile_s = game::Config::tile_Size;

            switch (push_direction) {
                case Player::Up:    move_offset_pixels.y = -tile_s; break;
                case Player::Down:  move_offset_pixels.y =  tile_s; break;
                case Player::Left:  move_offset_pixels.x = -tile_s; break;
                case Player::Right: move_offset_pixels.x =  tile_s; break;
                default: return; // Keine Bewegung für diagonale oder undefinierte Richtungen
            }

            // Zielposition für diesen einen Bewegungsschritt (obere linke Ecke des visuellen Sprites)
            Vector2 next_visual_top_left = {position_.x + move_offset_pixels.x, position_.y + move_offset_pixels.y};

            // Berechne die Hitbox an der Zielposition
            Rectangle next_step_hitbox = {
                next_visual_top_left.x + game::Config::Puzzle1::movable_Wall_Hitbox_Offset_X,
                next_visual_top_left.y + game::Config::Puzzle1::movable_Wall_Hitbox_Offset_Y,
                game::Config::Puzzle1::movable_Wall_Hitbox_Width,
                game::Config::Puzzle1::movable_Wall_Hitbox_Height
            };

            // Kollisionsprüfung auf dem Zielfeld
            bool collision_at_next_step = false;
            for (const auto* other_wall : all_walls) {
                if (other_wall == this) continue; // Nicht mit sich selbst kollidieren
                if (game::core::check_collision(next_step_hitbox, other_wall->Get_Rect())) {
                    collision_at_next_step = true;
                    std::cout << "Movable_Wall: Path blocked by another wall." << std::endl;
                    break;
                }
            }
            // TODO: Hier später auch Kollision mit Spieler (außer dem schiebenden) und Gegnern auf dem Zielfeld prüfen.

            if (!collision_at_next_step) {
                is_moving_ = true;
                sprite_render_destination_on_tile_ = next_visual_top_left; // Ziel für die visuelle Bewegung
                // Die logische Position/Hitbox wird verschoben, wenn die visuelle Bewegung abgeschlossen ist.
                // ODER: "Reserviere" das Zielfeld (komplexer, für später)
                std::cout << "Movable_Wall: Movement initiated towards (" << next_visual_top_left.x << ", " << next_visual_top_left.y << ")" << std::endl;
            } else {
                // Bewegung nicht möglich, Push-Versuch zurücksetzen, falls nötig
                current_push_delay_timer_ = 0.0f; // Erlaube neuen Push-Versuch
            }
        }

        // Update-Methode für die Movable_Wall (wird jeden Frame aufgerufen)
        void Update(float delta_time, const std::vector<Wall*>& all_walls /* ggf. weitere Parameter */) override {
            if (is_solved_) return; // Gelöste Wände machen nichts mehr

            // Cooldown-Timer aktualisieren
            if (current_move_cooldown_timer_ > 0.0f) {
                current_move_cooldown_timer_ -= delta_time;
                if (current_move_cooldown_timer_ < 0.0f) current_move_cooldown_timer_ = 0.0f;
            }

            // Push-Delay-Timer aktualisieren (wenn er gestartet wurde)
            if (current_push_delay_timer_ > 0.0f) {
                current_push_delay_timer_ -= delta_time;
                 if (current_push_delay_timer_ < 0.0f) current_push_delay_timer_ = 0.0f;
            }

            // Visuelle Bewegung, wenn is_moving_ true ist
            if (is_moving_) {
                Vector2 direction_to_target = {
                    sprite_render_destination_on_tile_.x - position_.x,
                    sprite_render_destination_on_tile_.y - position_.y
                };
                float distance_to_target = Vector2Length(direction_to_target);
                float move_step_pixels = game::Config::Puzzle1::movable_Wall_Sprite_Movement_Speed * delta_time;

                if (distance_to_target <= move_step_pixels) {
                    // Ziel erreicht
                    position_ = sprite_render_destination_on_tile_; // Setze exakte Position
                    Update_Hitbox_Rectangle();                       // Aktualisiere Hitbox-Position
                    is_moving_ = false;
                    current_move_cooldown_timer_ = game::Config::Puzzle1::movable_Wall_Move_Cooldown;
                    std::cout << "Movable_Wall: Movement finished. Position: (" << position_.x << ", " << position_.y << ")" << std::endl;
                    Check_If_Solved(); // Prüfe, ob die Wand jetzt an ihrer finalen Zielposition ist
                } else {
                    // Bewege dich in Richtung Ziel
                    Vector2 normalized_dir = Vector2Normalize(direction_to_target);
                    position_.x += normalized_dir.x * move_step_pixels;
                    position_.y += normalized_dir.y * move_step_pixels;
                    Update_Hitbox_Rectangle(); // Hitbox bewegt sich mit der visuellen Position
                }
            }
        }

        void Check_If_Solved() {
            if (is_solved_) return; // Nicht erneut prüfen, wenn schon gelöst

            // Vergleiche die aktuelle (obere linke) Position mit der Zielposition.
            // Fließkommavergleiche benötigen eine kleine Toleranz (Epsilon).
            float epsilon = 0.1f;
            if (fabs(position_.x - target_position_pixel_.x) < epsilon &&
                fabs(position_.y - target_position_pixel_.y) < epsilon) {
                is_solved_ = true;
                std::cout << "Movable_Wall at (" << position_.x << ", " << position_.y << ") IS SOLVED!" << std::endl;
                // Hier später: Sprite zu "aktiv" wechseln
                // Hier später: Benachrichtigung an ein Rätsel-Management-System
            }
        }

        bool Is_Solved() const {
            return is_solved_;
        }

        bool Is_Currently_Moving() const { // Hilfsmethode für Spieler-Kollision
            return is_moving_;
        }


        // Die Draw-Methode wird komplexer wegen der geteilten Sprites (Top/Bottom)
        // Fürs Erste eine einfache farbige Darstellung:
        void Draw() const override {
            Color current_color = BLUE; // Standardfarbe für Movable_Wall
            if (is_solved_) {
                current_color = GREEN;
            } else if (is_moving_) {
                current_color = YELLOW;
            }

            // Zeichne die visuelle Repräsentation (2x2 Kacheln)
            // position_ ist die obere linke Ecke des visuellen Sprites.
            DrawRectangleV(position_,
                           {game::Config::Puzzle1::movable_Wall_Visual_Width, game::Config::Puzzle1::movable_Wall_Visual_Height},
                           current_color);

            // Optionale Debug-Anzeige der Hitbox
            // DrawRectangleLinesEx(rect_, 2, VIOLET);
        }

        // Später:
        // void Draw_Bottom_Part() const;
        // void Draw_Top_Part() const;
    };

} // namespace game::coreyy
#endif //WALL_COLLISIONS_H

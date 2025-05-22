#ifndef PLAYERINPUT_H
#define PLAYERINPUT_H

#include "raylib.h"

class Player{
public:
    // Konstroktor initialisiert Spieler mit Start Position und Bewegungsgeschwindigkeit
    Player(Vector2 start_Position, float move_Speed);

    // Wird mit jedem Frame aufgerufen, um Position zu aktualisieren
    void Update();

    //Zeichnet den Spieler
    void Draw();
    Vector2 GetPosition() const { return position; }

private:

    //Aktuelle Position des Spielers
    Vector2 position;

    //Bewegungsgeschwindigkeit
    float speed;

    //Tastenzuweoßung für die Bewegung
    int key_Up;
    int key_Down;
    int key_Left;
    int key_Right;

};

#endif //PLAYERINPUT_H

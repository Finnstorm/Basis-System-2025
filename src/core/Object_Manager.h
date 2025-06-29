#pragma once

#include <vector>
#include <memory>
#include <algorithm> // Für std::remove
#include "Actor.h"



class Object_Manager
{
private:
    // Hier speichern Zeiger auf die verwalteten Actor-Objekte.
    // Der Manager ist nicht Eigentümer dieser Objekte sie werden woanders erstellt und gelöscht (z.B in Scene).
    std::vector<game::core::Actor*> actors_to_manage;

public:
    // Konstruktor:
    Object_Manager(Rectangle world_Bounds);

    // Methoden zum Registrieren und Deregistrieren von Actor-Objekten

    void Regist_Object(game::core::Actor* actor); // Verwaltet Actor, nicht Collidable
    void Unregist_Object(game::core::Actor* actor); // Verwaltet Actor, nicht Collidable

    // Hauptmethode, die alle verwalteten Actor-Objekte aktualisiert und zeichnet.

    void Draw();

    // Methode, um alle Actor-Objekte aus dem Manager zu entfernen (z.B. bei einem Szenenwechsel).
    void Clear_All_Objects();

};


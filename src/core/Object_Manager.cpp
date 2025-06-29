#include "Object_Manager.h"
#include <iostream>

// Konstruktor
Object_Manager::Object_Manager(Rectangle world_Bounds)
{

}

// Methode zum Registrieren eines Actor-Objekts
void Object_Manager::Regist_Object(game::core::Actor* actor)
{
    actors_to_manage.push_back(actor);

}

// Methode zum Deregistrieren eines Actor-Objekts
void Object_Manager::Unregist_Object(game::core::Actor* actor)
{
    // Entfernen des Actors
    actors_to_manage.erase(std::remove(actors_to_manage.begin(), actors_to_manage.end(), actor), actors_to_manage.end());

}

// Hauptmethode, die alle verwalteten Actor-Objekte aktualisiert und zeichnet.
void Object_Manager::Draw()
{

    // Methode um alle verwalteten Actors zu aktualisieren und zu zeichnen.


    for (game::core::Actor* actor : actors_to_manage)
    {
        if (actor) {
            actor->Update(); // Ruft die Update-Methode des einzelnen Actors auf

            // Update in Actors benötigt ?
        }
    }


    // Achtung: Stage::Draw() zeichnet bereits Actors.
    // draw in actors benötigt !!!

    for (game::core::Actor* actor : actors_to_manage)
    {
        if (actor) { // Sicherheitscheck
            actor->Draw(); // Ruft die Draw-Methode des einzelnen Actors auf
        }
    }


}

// Methode um alle Actor-Objekte aus dem Manager zu entfernen
void Object_Manager::Clear_All_Objects()
{
    actors_to_manage.clear();

}


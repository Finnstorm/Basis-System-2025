//
// Created by $Will on 27.08.2025.
//

#ifndef FAIRYITEM_H
#define FAIRYITEM_H
#include "ItemBase.h"

class FairyItem : public ItemBase
{
public:
    // Braucht das aktuelle Level, um die richtige Grafik zu laden
    FairyItem(Vector2 position, int current_level);

    // Activate wird nie aufgerufen, kann also leer bleiben
    void Activate(Player_Base_Class* player) override {}
};


#endif //FAIRYITEM_H

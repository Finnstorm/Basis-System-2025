//
// Created by $Will on 27.08.2025.
//

#ifndef TESTONEEDLE_H
#define TESTONEEDLE_H


#include "ItemBase.h"

class TestoNeedle : public ItemBase
{
public:
    TestoNeedle(Vector2 position);
    void Activate(Player_Base_Class* player) override;
};

#endif //TESTONEEDLE_H

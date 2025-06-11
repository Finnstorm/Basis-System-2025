//
// Created by $Will on 15.05.2025.
//

#ifndef Enemy_Base_Class_H
#define Enemy_Base_Class_H

#include <string>

namespace enemy
{
    class Enemy_Base_Class
    {
    protected:
      std::string enemy_Name;
      int enemy_Health;
      int enemy_Movement_Speed;
      int enemy_Damage;
      int enemy_Value; // Score etc.

        Rectangle enemy_Hitbox;

    public:

    //Konstruktor
    Enemy_Base_Class(std::string& name, int health, int movement_Speed, int damage, int value);

    //Destruktor
    virtual ~Enemy_Base_Class();

    //Getter
    std::string get_Name() const;
    int get_Health() const;
    int get_Movement_Speed() const;
    int get_Damage() const;
    int get_Value() const;

    //Setter
    void set_Health (int new_Health);
    //health ist erstmal der einzige Wert den wir während der Laufzeit durch Schaden verändern wollen

    //Methoden welche noch entwickelt werden müssen
    virtual void Enemy_Attack() = 0;
    virtual void Enemy_Take_Damage() = 0;
	virtual void Enemy_Draw() = 0;
    virtual void Enemy_Pathfinding() = 0;
    //Wir nutzen "= 0". Dadurch wird die Klasse abstrakt und MUSS überschrieben werden

    };
}

#endif //Enemy_Base_Class_H

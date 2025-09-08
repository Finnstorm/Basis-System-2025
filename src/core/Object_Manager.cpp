//
// Created by Kruse on 02/07/2025.
//

#include "Object_Manager.h"


#include <iostream>

Object_Manager::Object_Manager()
{

}

void Object_Manager::AddObject(Collidable* object)
{
    managed_objects.push_back(object);
}

void Object_Manager::RemoveObject(Collidable* object)
{
    managed_objects.erase(std::remove(managed_objects.begin(), managed_objects.end(), object), managed_objects.end());
}


void Object_Manager::ClearAllObjects()
{
    managed_objects.clear();

}

void Object_Manager::Cleanup_Objects(const std::function<void(Collidable*)>& on_object_cleaned)
{
    auto new_end = std::remove_if(managed_objects.begin(), managed_objects.end(),
    [&](Collidable* obj)
    {
        if (obj->Is_Marked_For_Destruction())
        {
            on_object_cleaned(obj);

            delete obj;
            return true;
        }
        return false;
    });
    managed_objects.erase(new_end, managed_objects.end());
}

void Object_Manager::AddObjectDeferred(Collidable* object)
{
    if (object)
    {
        objects_to_add.push_back(object);
    }
}

void Object_Manager::ProcessAdditions()
{
    if (objects_to_add.empty())
    {
        return;
    }
    managed_objects.insert(managed_objects.end(), objects_to_add.begin(), objects_to_add.end());

    objects_to_add.clear();
}
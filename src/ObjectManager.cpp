#include "ObjectManager.hpp"

ObjectManager::ObjectManager(){ }

ObjectManager::~ObjectManager(){ 

    for (auto &object : objects){   // delete all objects
        delete object;
    }

}


void ObjectManager::AddObject(Object *object){

    objects.push_back(object);
}

void ObjectManager::RenderAllObjects(){

    for (auto &object : objects){   // render all objects
        object->Draw();
    }

}


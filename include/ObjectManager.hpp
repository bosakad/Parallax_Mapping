/** @file ObjectManager.hpp
 *  @brief Class for maintaining objects in the scene
 *  
 *  Class for maintaining objects in the scene.
 *
 *  @author Adam Bosak
 *  @bug No known bugs.
 */

#ifndef OBJECTMANAGER_HPP
#define OBJECTMANAGER_HPP

#include <vector>
#include "Object.hpp"

class ObjectManager{

public:

    ObjectManager();
    ~ObjectManager();

    void RenderAllObjects();

    void AddObject(Object *object);
    

private: 

    std::vector<Object *> objects;

};



#endif
//
// Created by Yomi Ndu on 20/07/2024.
//

#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H
#include "Body.h"
#include "Contact.h"


struct CollisionDetection {
    static bool IsColliding(Body *a, Body *b, Contact &contact);
    static bool IsCollidingCircleCircle(Body *a, Body *b, Contact &contact);
    static bool IsCollidingPolygonPolygon(Body *a, Body *b, Contact &contact);
};


#endif //COLLISIONDETECTION_H

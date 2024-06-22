//
// Created by Yomi Ndu on 11/06/2024.
//

#ifndef INC_2DPHYSICSENGINE_BODY_H
#define INC_2DPHYSICSENGINE_BODY_H


#include "Shape.h"
#include "Vec2.h"

struct Body {
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    Vec2 sumForces;

    float mass;
    float invMass;

    Shape* shape = nullptr;

    Body(const Shape& shape, float x, float y, float mass);
    ~Body();

    void AddForce(const Vec2 &force);
    void ClearForces();

    void Integrate(float dt);
};


#endif //INC_2DPHYSICSENGINE_BODY_H

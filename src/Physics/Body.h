//
// Created by Yomi Ndu on 11/06/2024.
//

#ifndef INC_2DPHYSICSENGINE_BODY_H
#define INC_2DPHYSICSENGINE_BODY_H


#include "Shape.h"
#include "Vec2.h"

struct Body {
    bool isColliding;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    Vec2 sumForces;
    float sumTorque;

    float mass;
    float invMass;
    float momentI;
    float invMomentI;

    float rotation;
    float angularVelocity;
    float angluarAcceleration;

    Shape *shape = nullptr;

    Body(const Shape &shape, float x, float y, float mass);
    ~Body();

    void AddForce(const Vec2 &force);
    void AddTorque(float torque);

    void ClearForces();
    void ClearTorque();

    void IntegrateLinear(float dt);
    void IntegrateAngular(float timeDelta);

    void Update(float timeDelta);

    bool IsStatic() const;
};


#endif //INC_2DPHYSICSENGINE_BODY_H

//
// Created by Yomi Ndu on 11/06/2024.
//

#include "Body.h"

#include <iostream>


Body::Body(const Shape &shape, const float x, const float y, const float mass) {
    this->isColliding = false;
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->velocity = Vec2(0, 0);
    this->acceleration = Vec2(0, 0);
    this->rotation = 0.0;
    this->angularVelocity = 0.0;
    this->angluarAcceleration = 0.0;
    this->sumForces = Vec2(0, 0);
    this->sumTorque = 0.0;
    this->mass = mass;
    this->restitution = 1.0;
    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    } else {
        this->invMass = 0.0;
    }

    this->momentI = shape.GetMomentOfIntertia() * mass;
    if (this->momentI != 0.0) {
        this->invMomentI = 1.0 / this->momentI;
    } else {
        this->invMomentI = 0.0;
    }
}

Body::~Body() {
    delete shape;
    std::cout << "Body destructor called" << std::endl;
}


void Body::AddForce(const Vec2 &force) {
    sumForces += force;
}

void Body::AddTorque(float torque) {
    sumTorque += torque;
}


void Body::ClearForces() {
    sumForces = Vec2(0, 0);
}

void Body::ClearTorque() {
    sumTorque = 0.0;
}

void Body::ApplyImpulse(const Vec2 &j) {
    if (this->IsStatic()) {
        return;
    }

    velocity += j * invMass;
}

void Body::IntegrateLinear(const float dt) {
    if (IsStatic()) {
        return;
    }
    acceleration = sumForces * invMass;
    velocity += acceleration * dt;
    position += velocity * dt;
    ClearForces();
}

void Body::IntegrateAngular(float timeDelta) {
    if (IsStatic()) {
        return;
    }
    angluarAcceleration = sumTorque * timeDelta;
    angularVelocity += angluarAcceleration * timeDelta;
    rotation += angluarAcceleration * timeDelta;
    ClearTorque();
}

void Body::Update(float timeDelta) {
    this->IntegrateLinear(timeDelta);
    this->IntegrateAngular(timeDelta);

    if (this->shape->GetType() == POLYGON || this->shape->GetType() == BOX) {
        auto *polygonShape = (PolygonShape *) this->shape;
        polygonShape->UpdateVertices(this->rotation, this->position);
    }
}

bool Body::IsStatic() const {
    const float epsilon = 0.005f;
    return fabs(invMass) < epsilon;
}

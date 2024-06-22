//
// Created by Yomi Ndu on 11/06/2024.
//

#include "Body.h"

#include <iostream>


Body::Body(const Shape &shape, const float x, const float y, const float mass) {
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->mass = mass;
    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    } else {
        this->invMass = 0.0;
    }
}

Body::~Body() {
    delete shape;
    std::cout << "Body destructor called" << std::endl;
}


void Body::AddForce(const Vec2 &force) {
    sumForces += force;
}

void Body::ClearForces() {
    sumForces = Vec2(0, 0);
}


void Body::Integrate(float dt) {
    // todo: find the acceleration based on the forced being applied and the mass of the body
}

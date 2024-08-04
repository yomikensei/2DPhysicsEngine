//
// Created by Yomi Ndu on 21/07/2024.
//

#include "Contact.h"


void Contact::ResolvePenetration() const {
    if (a->IsStatic() && b->IsStatic()) {
        return;
    }

    const float deltaAPos = depth / (a->invMass + b->invMass) * a->invMass;
    const float deltaBPos = depth / (a->invMass + b->invMass) * b->invMass;

    a->position -= normal * deltaAPos;
    b->position += normal * deltaBPos;
}

void Contact::ResolveCollision() const {
    this->ResolvePenetration();

    float e = std::min(a->restitution, b->restitution);

    Vec2 vRel = a->velocity - b->velocity;
    float vRelDotNormal = vRel.Dot(normal);

    const Vec2 impulseDirection = normal;
    const float impulseMagnitude = -(1 + e) * vRelDotNormal / (a->invMass + b->invMass);

    Vec2 j = impulseDirection * impulseMagnitude;

    a->ApplyImpulse(j);
    b->ApplyImpulse(-j);
}

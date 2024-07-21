//
// Created by Yomi Ndu on 20/07/2024.
//

#include "CollisionDetection.h"

#include "Contact.h"

bool CollisionDetection::IsColliding(Body *a, Body *b, Contact &contact) {
    const bool aIsCircle = a->shape->GetType() == CIRCLE;
    const bool bIsCircle = b->shape->GetType() == CIRCLE;

    if (aIsCircle && bIsCircle) {
        return IsCollidingCircleCircle(a, b, contact);
    }
    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body *a, Body *b, Contact &contact) {
    auto *aCircleShape = (CircleShape *) a->shape;
    auto *bCircleShape = (CircleShape *) b->shape;

    const Vec2 ab = b->position - a->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;

    bool isColliding = ab.MagnitudeSquared() <= pow(radiusSum, 2);
    if (!isColliding) {
        return false;
    }

    contact.a = a;
    contact.b = b;
    contact.normal = ab;
    contact.normal.Normalize();
    contact.start = b->position - contact.normal * bCircleShape->radius;
    contact.end = a->position + contact.normal * aCircleShape->radius;
    contact.depth = (contact.end - contact.start).Magnitude();

    return isColliding;
}

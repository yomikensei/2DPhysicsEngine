//
// Created by Yomi Ndu on 20/07/2024.
//

#include "CollisionDetection.h"

#include "Contact.h"

bool CollisionDetection::IsColliding(Body *a, Body *b, Contact &contact) {
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;
    bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
    bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;

    if (aIsCircle && bIsCircle) {
        return IsCollidingCircleCircle(a, b, contact);
    }
    if (aIsPolygon && bIsPolygon) {
        return IsCollidingPolygonPolygon(a, b, contact);
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


bool CollisionDetection::IsCollidingPolygonPolygon(Body *a, Body *b, Contact &contact) {
    const PolygonShape *aPolygonShape = (PolygonShape *) a->shape;
    const PolygonShape *bPolygonShape = (PolygonShape *) b->shape;

    Vec2 aAxis, bAxis;
    Vec2 aPoint, bPoint;

    float abSeparation = aPolygonShape->FindMinSeparation(*bPolygonShape, aAxis, aPoint);
    float baSeparation = bPolygonShape->FindMinSeparation(*aPolygonShape, bAxis, bPoint);

    if (abSeparation >= 0) {
        return false;
    }
    if (baSeparation >= 0) {
        return false;
    }

    contact.a = a;
    contact.b = b;

    if (abSeparation > baSeparation) {
        contact.depth = -abSeparation;
        contact.normal = aAxis.Normal();
        contact.start = aPoint;
        contact.end = aPoint + contact.normal * contact.depth;
    } else {
        contact.depth = -baSeparation;
        contact.normal = -bAxis.Normal();
        contact.start = bPoint - contact.normal * contact.depth;
        contact.end = bPoint;
    }
    return true;
}

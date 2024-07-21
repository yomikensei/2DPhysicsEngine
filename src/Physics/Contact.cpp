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

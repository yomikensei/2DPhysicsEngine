#include <iostream>
#include "Shape.h"


CircleShape::CircleShape(const float radius) {
    this->radius = radius;
    std::cout << "CircleShape constructor called" << std::endl;
}

CircleShape::~CircleShape() {
    std::cout << "CircleShape destructor called" << std::endl;
}

ShapeType CircleShape::GetType() const {
    return CIRCLE;
}

Shape *CircleShape::Clone() const {
    return new CircleShape(radius);
}

float CircleShape::GetMomentOfIntertia() const {
    return 0.5 * pow(radius, 2);;
}

PolygonShape::PolygonShape(const std::vector<Vec2> &vertices) {
    std::cout << "PolygonShape constructor called" << std::endl;
}

PolygonShape::~PolygonShape() {
    std::cout << "PolygonShape destructor called" << std::endl;
}

ShapeType PolygonShape::GetType() const {
    return POLYGON;
}

Shape *PolygonShape::Clone() const {
    return new PolygonShape(vertices);
}

float PolygonShape::GetMomentOfIntertia() const {
    return 0.0;
}

BoxShape::BoxShape(float width, float height) {
    std::cout << "BoxShape constructor called" << std::endl;
}

BoxShape::~BoxShape() {
    std::cout << "BoxShape destructor called" << std::endl;
}

ShapeType BoxShape::GetType() const {
    return BOX;
}

Shape *BoxShape::Clone() const {
    return new BoxShape(width, height);
}

float BoxShape::GetMomentOfIntertia() const {
    return 0.08333333333 * pow(width, 2) * pow(height, 2);
}

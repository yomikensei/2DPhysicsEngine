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
    return new PolygonShape(localVertices);
}

float PolygonShape::GetMomentOfIntertia() const {
    return 0.0;
}

Vec2 PolygonShape::EdgeAt(int index) const {
    int currVertex = index;
    int nextVertex = (index + 1) % worldVertices.size();
    return worldVertices[nextVertex] - worldVertices[currVertex];
}

float PolygonShape::FindMinSeparation(const PolygonShape &other, Vec2 &axis, Vec2 &point) const {
    float separation = std::numeric_limits<float>::lowest();

    for (int vThisIndex = 0; vThisIndex < worldVertices.size(); vThisIndex++) {
        Vec2 vThis = worldVertices[vThisIndex];
        Vec2 vAEdgeNormal = this->EdgeAt(vThisIndex).Normal();
        float minSeparation = std::numeric_limits<float>::max();
        Vec2 minVertex;
        for (auto vOther: other.worldVertices) {
            float projection = (vOther - vThis).Dot(vAEdgeNormal);
            if (projection < minSeparation) {
                minSeparation = projection;
                minVertex = vOther;
            }
            minSeparation = std::min(minSeparation, projection);
        }
        if (minSeparation > separation) {
            separation = minSeparation;
            axis = this->EdgeAt(vThisIndex);
            point = minVertex;
        }
    }
    return separation;
}

void PolygonShape::UpdateVertices(float angle, const Vec2 &position) {
    for (int i = 0; i < localVertices.size(); i++) {
        worldVertices[i] = localVertices[i].Rotate(angle);
        worldVertices[i] += position;
    }
}

BoxShape::BoxShape(float width, float height) {
    this->width = width;
    this->height = height;
    localVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
    localVertices.push_back(Vec2(+width / 2.0, -height / 2.0));
    localVertices.push_back(Vec2(+width / 2.0, +height / 2.0));
    localVertices.push_back(Vec2(-width / 2.0, +height / 2.0));

    worldVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
    worldVertices.push_back(Vec2(+width / 2.0, -height / 2.0));
    worldVertices.push_back(Vec2(+width / 2.0, +height / 2.0));
    worldVertices.push_back(Vec2(-width / 2.0, +height / 2.0));
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

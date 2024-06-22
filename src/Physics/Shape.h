#ifndef INC_2DPHYSICSENGINE_SHAPE_H
#define INC_2DPHYSICSENGINE_SHAPE_H


#include <vector>
#include "Vec2.h"

enum ShapeType {
    CIRCLE,
    POLYGON,
    BOX
};

struct Shape {
    virtual ~Shape() = default;
    virtual ShapeType GetType() const = 0;
    virtual Shape *Clone() const = 0;
};

struct CircleShape : public Shape {
    float radius;

    CircleShape(const float radius);
    virtual ~CircleShape();
    ShapeType GetType() const override;
    Shape *Clone() const override;
};

struct PolygonShape : public Shape {
    std::vector<Vec2> vertices;

    PolygonShape() = default;
    PolygonShape(const std::vector<Vec2>& vertices);
    virtual ~PolygonShape();
    ShapeType GetType() const override;
    Shape *Clone() const override;
};

struct BoxShape : public Shape {
    float width;
    float height;

    BoxShape(float width, float height);
    virtual ~BoxShape();
    ShapeType GetType() const override;
    Shape *Clone() const override;
};


#endif //INC_2DPHYSICSENGINE_SHAPE_H

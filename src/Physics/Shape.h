#ifndef INC_2DPHYSICSENGINE_SHAPE_H
#define INC_2DPHYSICSENGINE_SHAPE_H


#include "Vec2.h"
#include <vector>

enum ShapeType {
	CIRCLE,
	POLYGON,
	BOX
};

struct Shape {
	virtual ~Shape() = default;
	virtual ShapeType GetType() const = 0;
	virtual Shape *Clone() const = 0;
	virtual void UpdateVertices(float angle, const Vec2 &position) = 0;
	virtual float GetMomentOfIntertia() const = 0;
};

struct CircleShape : public Shape {
	float radius;

	CircleShape(const float radius);
	virtual ~CircleShape();
	ShapeType GetType() const override;
	Shape *Clone() const override;
	float GetMomentOfIntertia() const override;
	void UpdateVertices(float angle, const Vec2 &position) override;
};

struct PolygonShape : public Shape {
	float width;
	float height;

	std::vector<Vec2> localVertices;
	std::vector<Vec2> worldVertices;

	PolygonShape() = default;
	PolygonShape(const std::vector<Vec2> &vertices);
	virtual ~PolygonShape();
	ShapeType GetType() const override;
	Shape *Clone() const override;
	float GetMomentOfIntertia() const override;
	Vec2 EdgeAt(int index) const;
	float FindMinSeparation(const PolygonShape *other, int &indexRefEdge, Vec2 &supportPoint) const;
	int FindIncidentEdge(const Vec2 &normal) const;
	int ClipSegmentToLine(const std::vector<Vec2> &contactsIn, std::vector<Vec2> &contactsOut, const Vec2 &c0, const Vec2 &c1) const;
	void UpdateVertices(float angle, const Vec2 &position) override;
};

struct BoxShape : public PolygonShape {
	BoxShape(float width, float height);
	virtual ~BoxShape();
	ShapeType GetType() const override;
	Shape *Clone() const override;
	float GetMomentOfIntertia() const override;
};


#endif//INC_2DPHYSICSENGINE_SHAPE_H

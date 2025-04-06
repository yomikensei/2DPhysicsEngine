#include "Shape.h"
#include <iostream>


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
	return 0.5 * pow(radius, 2);
	;
}

void CircleShape::UpdateVertices(float angle, const Vec2 &position) {
}


PolygonShape::PolygonShape(const std::vector<Vec2> &vertices) {
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();


	for (auto vertex: vertices) {
		localVertices.push_back(vertex);
		worldVertices.push_back(vertex);

		minX = std::min(minX, vertex.x);
		maxX = std::max(maxX, vertex.x);
		minY = std::min(minY, vertex.y);
		maxY = std::max(maxY, vertex.y);
	}
	width = maxX - minX;
	height = maxY - minY;

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
	float acc0 = 0;
	float acc1 = 0;
	for (int i = 0; i < localVertices.size(); i++) {
		auto a = localVertices[i];
		auto b = localVertices[(i + 1) % localVertices.size()];
		auto cross = abs(a.Cross(b));
		acc0 += cross * (a.Dot(a) + b.Dot(b) + a.Dot(b));
		acc1 += cross;
	}
	return acc0 / 6 / acc1;
}

Vec2 PolygonShape::EdgeAt(int index) const {
	int currVertex = index;
	int nextVertex = (index + 1) % worldVertices.size();
	return worldVertices[nextVertex] - worldVertices[currVertex];
}

float PolygonShape::FindMinSeparation(const PolygonShape *other, int& indexRefEdge, Vec2 &supportPoint) const {
	float separation = std::numeric_limits<float>::lowest();

	for (int i = 0; i < other->worldVertices.size(); i++) {
		Vec2 vA = this->worldVertices[i];
		Vec2 normal = this->EdgeAt(i).Normal();

		float minSeparation = std::numeric_limits<float>::max();
		Vec2 minVertex;

		for (int j = 0; j < other->worldVertices.size(); j++) {
			Vec2 vB = other->worldVertices[j];
			float proj = (vB - vA).Dot(normal);
			if (proj < minSeparation) {
				minSeparation = proj;
				minVertex = vB;
			}
		}
		if (minSeparation > separation) {
			separation = minSeparation;
			indexRefEdge = i;
			supportPoint = minVertex;
		}
	}
	return separation;
}

int PolygonShape::FindIncidentEdge(const Vec2 &normal) const {
	int incidentEdge = -1;
	float minProjection = std::numeric_limits<float>::max();

	for (int i = 0; i < this->worldVertices.size(); i++) {
		auto currentEdgeNormal = this->EdgeAt(i).Normal();
		auto projection = currentEdgeNormal.Dot(normal);
		if (projection < minProjection) {
			minProjection = projection;
			incidentEdge = i;
		}
	}

	return incidentEdge;
}

int PolygonShape::ClipSegmentToLine(const std::vector<Vec2> &contactsIn, std::vector<Vec2> &contactsOut, const Vec2 &c0, const Vec2 &c1) const {
	int numOut = 0;

	Vec2 normal = (c1 - c0).Normalize();
	float distance0 = (contactsIn[0] - c0).Cross(normal);
	float distance1 = (contactsIn[1] - c0).Cross(normal);

	if (distance0 <= 0) {
		contactsOut[numOut++] = contactsIn[0];
	}
	if (distance1 <= 0) {
		contactsOut[numOut++] = contactsIn[1];
	}

	if (distance0 * distance1 < 0) {
		float totalDistance = distance0 - distance1;
		float t = distance0 / totalDistance;
		Vec2 contact = contactsIn[0] + (contactsIn[1] - contactsIn[0]) * t;
		contactsOut[numOut] = contact;
		numOut++;
	}
	return numOut;
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

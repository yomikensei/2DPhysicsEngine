//
// Created by Yomi Ndu on 11/06/2024.
//

#include "Body.h"
#include "../Graphics.h"
#include <SDL_image.h>

#include <iostream>


Body::Body(const Shape &shape, const float x, const float y, const float mass) {
	this->shape = shape.Clone();
	this->position = Vec2(x, y);
	this->velocity = Vec2(0, 0);
	this->acceleration = Vec2(0, 0);
	this->rotation = 0.0;
	this->angularVelocity = 0.0;
	this->angularAcceleration = 0.0;
	this->sumForces = Vec2(0, 0);
	this->sumTorque = 0.0;
	this->restitution = 1.0;
	this->friction = 0.7;
	this->mass = mass;
	if (mass != 0.0) {
		this->invMass = 1.0 / mass;
	} else {
		this->invMass = 0.0;
	}
	momentI = shape.GetMomentOfIntertia() * mass;
	if (momentI != 0.0) {
		this->inverseMomentI = 1.0 / momentI;
	} else {
		this->inverseMomentI = 0.0;
	}
	std::cout << "Body constructor called!" << std::endl;
}

Body::~Body() {
	delete shape;
	SDL_DestroyTexture(texture);
	std::cout << "Body destructor called" << std::endl;
}


void Body::SetTexture(const char *textureFileName) {
	SDL_Surface *surface = IMG_Load(textureFileName);
	if (surface) {
		texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
		SDL_FreeSurface(surface);
	}
}


void Body::AddForce(const Vec2 &force) {
	sumForces += force;
}

void Body::AddTorque(float torque) {
	sumTorque += torque;
}


void Body::ClearForces() {
	sumForces = Vec2(0, 0);
}

void Body::ClearTorque() {
	sumTorque = 0.0;
}

Vec2 Body::LocalSpaceToWorldSpace(const Vec2 &point) const {
	return point.Rotate(rotation) + position;
}

Vec2 Body::WorldSpaceToLocalSpace(const Vec2 &point) const {
	float translatedX = point.x - position.x;
	float translatedY = point.y - position.y;
	float rotatedX = cos(-rotation) * translatedX - sin(-rotation) * translatedY;
	float rotatedY = cos(-rotation) * translatedY + sin(-rotation) * translatedX;
	return Vec2(rotatedX, rotatedY);
}


void Body::ApplyImpulseLinear(const Vec2 &j) {
	if (IsStatic()) {
		return;
	}
	velocity += j * invMass;
}


void Body::ApplyImpulseAngular(const float j) {
	if (IsStatic()) {
		return;
	}
	angularVelocity += j * inverseMomentI;
}

void Body::ApplyImpulseAtPoint(const Vec2 &j, const Vec2 &r) {
	if (IsStatic()) {
		return;
	}
	velocity += j * invMass;
	angularVelocity += r.Cross(j) * inverseMomentI;
}


void Body::IntegrateForces(const float timeDelta) {
	if (IsStatic()) {
		return;
	}

	acceleration = sumForces * invMass;
	velocity += acceleration * timeDelta;
	angularAcceleration = sumTorque * inverseMomentI;
	angularVelocity += angularAcceleration * timeDelta;

	ClearForces();
	ClearTorque();
}

void Body::IntegrateVelocities(const float timeDelta) {
	if (IsStatic()) {
		return;
	}

	position += velocity * timeDelta;
	rotation += angularVelocity * timeDelta;
	shape->UpdateVertices(rotation, position);
}


bool Body::IsStatic() const {
	const float epsilon = 0.005f;
	return fabs(invMass) < epsilon;
}

//
// Created by Yomi Ndu on 11/06/2024.
//

#ifndef INC_2DPHYSICSENGINE_BODY_H
#define INC_2DPHYSICSENGINE_BODY_H


#include <SDL_render.h>
#include "Shape.h"
#include "Vec2.h"

struct Body {
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;

	Vec2 sumForces;
	float sumTorque;

	float mass;
	float invMass;
	float momentI;
	float inverseMomentI;

	float rotation;
	float angularVelocity;
	float angularAcceleration;

	float friction;
	float restitution;

	Shape *shape = nullptr;
	SDL_Texture *texture = nullptr;

	Body(const Shape &shape, float x, float y, float mass);
	~Body();

	void AddForce(const Vec2 &force);
	void AddTorque(float torque);

	void ClearForces();
	void ClearTorque();

	void ApplyImpulseLinear(const Vec2 &j);
	void ApplyImpulseAngular(const float j);
	void ApplyImpulseAtPoint(const Vec2 &j, const Vec2 &r);

	void IntegrateForces(const float timeDelta);
	void IntegrateVelocities(const float timeDelta);

	void Update(float timeDelta);

	bool IsStatic() const;

	void SetTexture(const char *textureFileName);

	Vec2 LocalSpaceToWorldSpace(const Vec2 &point) const;
	Vec2 WorldSpaceToLocalSpace(const Vec2 &point) const;
};


#endif //INC_2DPHYSICSENGINE_BODY_H

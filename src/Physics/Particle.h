//
// Created by Yomi Ndu on 31/03/2024.
//

#ifndef INC_2DPHYSICSENGINE_PARTICLE_H
#define INC_2DPHYSICSENGINE_PARTICLE_H


#include "Vec2.h"

struct Particle {
	int radius;
	float mass;
	float invMass;

	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;
	Vec2 sumForces;

	Particle(float x, float y, float mass);
	~Particle();

	void AddForce(const Vec2 &force);
	void ClearForces();
	void Integrate(float timeDelta);
};


#endif //INC_2DPHYSICSENGINE_PARTICLE_H

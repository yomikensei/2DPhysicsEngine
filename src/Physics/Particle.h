//
// Created by Yomi Ndu on 31/03/2024.
//

#ifndef INC_2DPHYSICSENGINE_PARTICLE_H
#define INC_2DPHYSICSENGINE_PARTICLE_H


#include "Vec2.h"

struct Particle {
	int radius;
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;

	float mass;

	Particle(float x, float y, float mass);
	~Particle();
};


#endif //INC_2DPHYSICSENGINE_PARTICLE_H

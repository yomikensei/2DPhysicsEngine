//
// Created by Yomi Ndu on 06/04/2024.
//

#ifndef INC_2DPHYSICSENGINE_FORCE_H
#define INC_2DPHYSICSENGINE_FORCE_H

#include "./Vec2.h"
#include "./Particle.h"


struct Force {
	static Vec2 GenerateDragForce(const Particle &particle, float k);
	static Vec2 GenerateFrictionForce(const Particle &particle, float k);
	static Vec2 GenerateGravitationalForce(const Particle& a, const Particle& b, float G);
	static Vec2 GenerateSpringForce(const Particle& particle, Vec2 anchor, float restLength, float k);
	static Vec2 GenerateSpringForce(const Particle& particleA, const Particle& particleB, float restLength, float k);

};


#endif //INC_2DPHYSICSENGINE_FORCE_H

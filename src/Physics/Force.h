//
// Created by Yomi Ndu on 06/04/2024.
//

#ifndef INC_2DPHYSICSENGINE_FORCE_H
#define INC_2DPHYSICSENGINE_FORCE_H

#include "./Vec2.h"
#include "./Body.h"


struct Force {
	static Vec2 GenerateDragForce(const Body& body, float k);
	static Vec2 GenerateFrictionForce(const Body& body, float k);
	static Vec2 GenerateSpringForce(const Body& body, Vec2 anchor, float restLength, float k);
	static Vec2 GenerateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance);

};


#endif //INC_2DPHYSICSENGINE_FORCE_H

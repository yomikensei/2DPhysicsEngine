//
// Created by Yomi Ndu on 06/04/2024.
//

#include "Force.h"

Vec2 Force::GenerateDragForce(const Particle &particle, float k) {
	Vec2 dragForce = Vec2(0, 0);
	if (particle.velocity.MagnitudeSquared() > 0) {
		Vec2 dragDirection = particle.velocity.UnitVector() * -1.0;
		float dragMagnitude = k * particle.velocity.MagnitudeSquared();
		dragForce = dragDirection * dragMagnitude;
	}
	return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Particle &particle, float k) {
	Vec2 frictionForce = particle.velocity.UnitVector() * -1 * k;
	return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Particle &a, const Particle &b, float G) {
	Vec2 d = (b.position - a.position);
	float distanceSquared = d.MagnitudeSquared();
	float attractionMagnitude = G * (a.mass * b.mass) / distanceSquared;

	Vec2 attractionForce = d.UnitVector() * attractionMagnitude;
	return attractionForce;
}

Vec2 Force::GenerateSpringForce(const Particle &particle, Vec2 anchor, float restLength, float k) {
	Vec2 d = particle.position - anchor;
	float displacement = d.Magnitude() - restLength;
	Vec2 springDirection = d.UnitVector();
	float springMagnitude = -k * displacement;
	Vec2 springForce = springDirection * springMagnitude;
	return springForce;
}

Vec2 Force::GenerateSpringForce(const Particle &particleA, const Particle &particleB, float restLength, float k) {
	Vec2 d = particleA.position - particleB.position;
	float displacement = d.Magnitude() - restLength;
	Vec2 springDirection = d.UnitVector();
	float springMagnitude = -k * displacement;
	Vec2 springForce = springDirection * springMagnitude;
	return springForce;}
